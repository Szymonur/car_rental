<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';

const route = useRoute();
const router = useRouter();

const rentalId = parseInt(route.params.id); // Get rental ID from route
const rental = ref(null); // Store rental details
const responseMessage = ref("");
const validationMessage = ref(""); // For validation errors

// Fetch rental details
const fetchRental = async () => {
  try {
    const response = await fetch(`http://localhost:18080/rentals/${rentalId}`);
    if (response.ok) {
      rental.value = await response.json();
      // Ensure that is_accepted is treated as a boolean (true/false)
      rental.value.is_accepted = rental.value.is_accepted === true || rental.value.is_accepted === 'true' || rental.value.is_accepted === 1;
    } else {
      console.error("Failed to fetch rental.");
    }
  } catch (error) {
    console.error("Error fetching rental:", error);
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

  if (startDate < today) {
    validationMessage.value = "Rental start date cannot be in the past.";
    return false;
  }

  if (endDate <= startDate) {
    validationMessage.value = "Rental end date must be after the start date.";
    return false;
  }

  // Fetch existing rentals for the same car
  try {
    const response = await fetch(`http://localhost:18080/rentals/car/${rental.value.car_vin}`);
    if (response.ok) {
      const existingRentals = await response.json();

      // Check for overlap with other rentals (excluding the current rental)
      const overlap = existingRentals.some((existingRental) => {
        // Skip the current rental being edited
        if (existingRental.id === rentalId) return false;

        const existingStart = new Date(existingRental.rental_start).setHours(0, 0, 0, 0);
        const existingEnd = new Date(existingRental.rental_end).setHours(0, 0, 0, 0);

        return (
          (startDate >= existingStart && startDate <= existingEnd) || // Start date overlaps
          (endDate >= existingStart && endDate <= existingEnd) || // End date overlaps
          (startDate <= existingStart && endDate >= existingEnd) // Full overlap
        );
      });

      if (overlap) {
        validationMessage.value = "The selected dates overlap with an existing rental for this car.";
        return false;
      }
    } else {
      console.error("Failed to fetch existing rentals for validation.");
      validationMessage.value = "An error occurred while validating rental dates.";
      return false;
    }
  } catch (error) {
    console.error("Error validating rental dates:", error);
    validationMessage.value = "An error occurred while validating rental dates.";
    return false;
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
      <input type="date" id="rental_start" v-model="rental.rental_start" />

      <label for="rental_end">Rental End:</label>
      <input type="date" id="rental_end" v-model="rental.rental_end" />

      <label for="is_accepted">Is Accepted:</label>
      <input type="checkbox" id="is_accepted" v-model="rental.is_accepted" />

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
</style>
