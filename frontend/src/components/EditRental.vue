<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';

const route = useRoute();
const router = useRouter();

const rentalId = parseInt(route.params.id); // Get rental ID from route
const rental = ref(null); // Store rental details
const responseMessage = ref("");
const validationMessage = ref(""); // For validation errors
const dailyRate = 50; // Assume a fixed daily rental rate for simplicity

// Fetch rental details
const fetchRental = async () => {
  try {
    const response = await fetch(`http://localhost:18080/rentals/${rentalId}`);
    if (response.ok) {
      rental.value = await response.json();
      // Ensure that is_accepted, if_paid, if_returned, and if_issued are treated as booleans (true/false)
      rental.value.is_accepted = Boolean(rental.value.is_accepted);
      rental.value.if_paid = Boolean(rental.value.if_paid);
      rental.value.if_returned = Boolean(rental.value.if_returned);
      rental.value.if_issued = Boolean(rental.value.if_issued);

      // Calculate the current cost on component mount
      updateRentalCost();
    } else {
      console.error("Failed to fetch rental.");
    }
  } catch (error) {
    console.error("Error fetching rental:", error);
  }
};

// Calculate total cost
const calculateCost = () => {
  if (!rental.value?.rental_start || !rental.value?.rental_end) return 0;

  const startDate = new Date(rental.value.rental_start);
  const endDate = new Date(rental.value.rental_end);
  const days = Math.max(1, Math.ceil((endDate - startDate) / (1000 * 60 * 60 * 24))); // At least 1 day

  return days * dailyRate;
};

// Update the rental cost when dates change
const updateRentalCost = () => {
  try {
    rental.value.total_cost = calculateCost(); // Ensure cost is calculated and updated
  } catch (error) {
    console.error("Error updating rental cost:", error);
  }
};

// Validate rental dates
const validateRental = async () => {
  const today = new Date().setHours(0, 0, 0, 0);
  const startDate = new Date(rental.value.rental_start).setHours(0, 0, 0, 0);
  const endDate = new Date(rental.value.rental_end).setHours(0, 0, 0, 0);

  if (!rental.value.rental_start || !rental.value.rental_end) {
    validationMessage.value = "Both start and end dates are required.";
    return false;
  }


  if (endDate <= startDate) {
    validationMessage.value = "Rental end date must be after the start date.";
    return false;
  }

  if (startDate < today) {
    validationMessage.value = "Rental start date is in the past.";
    return true;
  }

  validationMessage.value = ""; // Clear validation message if valid
  return true;
};

// Update rental
const updateRental = async () => {
  if (!await validateRental()) {
    return; // Stop if validation fails
  }

  try {
    rental.value.total_cost = calculateCost(); // Ensure cost is up to date

    const response = await fetch(`http://localhost:18080/rentals/${rentalId}`, {
      method: 'PUT',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(rental.value),
    });

    if (response.ok) {
      responseMessage.value = "Rental updated successfully!";
    } else {
      responseMessage.value = "Failed to update rental.";
    }
  } catch (error) {
    console.error("Error updating rental:", error);
    responseMessage.value = "An error occurred.";
  }
};

onMounted(() => {
  fetchRental(); // Load rental details on mount
});
</script>


<template>
  <div>
    <form v-if="rental">
      <h1>Edit Rental #{{ rentalId }}</h1>

      <label for="rental_start">Rental Start:</label>
      <input type="date" id="rental_start" v-model="rental.rental_start" @change="updateRentalCost" />

      <label for="rental_end">Rental End:</label>
      <input type="date" id="rental_end" v-model="rental.rental_end" @change="updateRentalCost" />

      <div class="form-row">
        <label for="is_accepted">Is Accepted:</label>
        <input type="checkbox" id="is_accepted" v-model="rental.is_accepted" />

        <label for="if_paid">If Paid:</label>
        <input type="checkbox" id="if_paid" v-model="rental.if_paid" />

        <label for="if_returned">If Returned:</label>
        <input type="checkbox" id="if_returned" v-model="rental.if_returned" />

        <label for="if_issued">If Issued:</label>
        <input type="checkbox" id="if_issued" v-model="rental.if_issued" />
      </div>

      <p>Total Cost: <strong>{{ rental.total_cost }} $</strong></p>

      <!-- Display validation error -->
      <p class="validation-error" v-if="validationMessage">{{ validationMessage }}</p>

      <button class="button-primary" type="button" @click="updateRental">Save Changes</button>

      <p v-if="responseMessage">{{ responseMessage }}</p>
    </form>
  </div>
</template>

<style scoped>
.validation-error {
  color: red;
  margin-top: 10px;
}
.form-row {
  display: flex; 
  align-items: center;
  gap: 10px;
  padding: 10px 0;
}
.button-primary {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 10px;
  cursor: pointer;
}
.button-primary:hover {
  background-color: #0056b3;
}
</style>
