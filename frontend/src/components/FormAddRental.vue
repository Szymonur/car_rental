<script setup>
import { ref, onMounted, computed } from "vue";

const formData = ref({
  rental_id: null,
  rental_start: "",
  rental_end: "",
  is_accepted: false,
  car_vin: "",
  user_id: "",
  total_cost: 0,
  if_paid: false,
  if_issued: false,  // New field
  if_returned: false, // New field
});

const users = ref([]);
const vehicles = ref([]);
const rentals = ref([]);
const responseMessage = ref("");
const userSearch = ref("");
const vehicleSearch = ref("");
const validationMessage = ref("");

// Fetch users, vehicles, and rentals data
const fetchUsers = async () => {
  try {
    const response = await fetch("http://localhost:18080/users");
    if (response.ok) {
      users.value = await response.json();
    } else {
      console.error("Failed to fetch users.");
    }
  } catch (error) {
    console.error("Error fetching users:", error);
  }
};

const fetchVehicles = async () => {
  try {
    const response = await fetch("http://localhost:18080/vehicles");
    if (response.ok) {
      vehicles.value = await response.json();
    } else {
      console.error("Failed to fetch vehicles.");
    }
  } catch (error) {
    console.error("Error fetching vehicles:", error);
  }
};

const fetchRentals = async () => {
  try {
    const response = await fetch("http://localhost:18080/rentals");
    if (response.ok) {
      rentals.value = await response.json();
    } else {
      console.error("Failed to fetch rentals.");
    }
  } catch (error) {
    console.error("Error fetching rentals:", error);
  }
};

// Filtered users and vehicles
const filteredUsers = computed(() => {
  if (!userSearch.value.trim()) return users.value;
  return users.value.filter(user => {
    const fullName = `${user.first_name} ${user.last_name}`.toLowerCase();
    return (
      fullName.includes(userSearch.value.toLowerCase()) ||
      user.phone.includes(userSearch.value)
    );
  });
});

const filteredVehicles = computed(() => {
  const rentalStart = new Date(formData.value.rental_start).getTime();
  const rentalEnd = new Date(formData.value.rental_end).getTime();

  return vehicles.value.filter(vehicle => {
    // Check if vehicle is available and not overlapping with any rental
    if (!vehicle.is_available) return false;

    const hasConflict = rentals.value.some(rental => {
      const existingStart = new Date(rental.rental_start).getTime();
      const existingEnd = new Date(rental.rental_end).getTime();

      return (
        rental.car_vin === vehicle.vin &&
        rentalStart < existingEnd &&
        rentalEnd > existingStart
      );
    });

    return !hasConflict;
  }).filter(vehicle => 
    vehicle.vin.toLowerCase().includes(vehicleSearch.value.toLowerCase())
  );
});

// Fetch data on mount
onMounted(() => {
  fetchUsers();
  fetchVehicles();
  fetchRentals();
});

// Calculate total cost based on selected vehicle and rental dates
const calculateTotalCost = () => {
  const vehicle = vehicles.value.find(v => v.vin === formData.value.car_vin);
  if (vehicle && formData.value.rental_start && formData.value.rental_end) {
    const startDate = new Date(formData.value.rental_start);
    const endDate = new Date(formData.value.rental_end);
    const rentalLength = (endDate - startDate) / (1000 * 60 * 60 * 24); // Days

    if (rentalLength >= 1) {
      formData.value.total_cost = rentalLength * vehicle.price_per_day;
    } else {
      formData.value.total_cost = 0; // Reset if invalid length
    }
  }
};

// Validate rental details
const validateRental = () => {
  const today = new Date().setHours(0, 0, 0, 0);
  const startDate = new Date(formData.value.rental_start).setHours(0, 0, 0, 0);
  const endDate = new Date(formData.value.rental_end).setHours(0, 0, 0, 0);

  if (!formData.value.rental_start || !formData.value.rental_end) {
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

  validationMessage.value = ""; // Clear validation message if valid
  return true;
};

// Submit form with validation
const submitForm = async () => {
  if (!validateRental()) {
    return; // Stop submission if validation fails
  }

  calculateTotalCost(); // Ensure total_cost is calculated before submission

  try {
    const response = await fetch("http://localhost:18080/add_rental", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(formData.value),
    });

    if (response.ok) {
      const result = await response.json();
      responseMessage.value = result.message;

      // Reset formData after successful submission
      formData.value = {
        rental_id: null,
        rental_start: "",
        rental_end: "",
        is_accepted: false,
        car_vin: "",
        user_id: "",
        if_paid: false,
        if_issued: false, // Reset new field
        if_returned: false, // Reset new field
        total_cost: 0,
      };

      // Refresh rentals to update availability
      fetchRentals();
    } else {
      responseMessage.value = "Error: Failed to submit the rental form.";
    }
  } catch (error) {
    console.error("Error submitting form:", error);
    responseMessage.value = "An error occurred while submitting the form.";
  }
};
</script>

<template>
  <div class="c-formAddRental">
    <form @submit.prevent="submitForm">
      <h1>Add Rental</h1>

      <label for="rental_start">Rental Start</label>
      <input type="date" id="rental_start" v-model="formData.rental_start" @change="calculateTotalCost" required />

      <label for="rental_end">Rental End</label>
      <input type="date" id="rental_end" v-model="formData.rental_end" @change="calculateTotalCost" required />

      <div class="c-formAddRental-row">
        <label for="is_accepted">Is Accepted</label>
        <input type="checkbox" id="is_accepted" v-model="formData.is_accepted" />

        <label for="if_paid">Is Paid</label>
        <input type="checkbox" id="if_paid" v-model="formData.if_paid" />

        <label for="if_issued">Is Issued</label>
        <input type="checkbox" id="if_issued" v-model="formData.if_issued" />

        <label for="if_returned">Is Returned</label>
        <input type="checkbox" id="if_returned" v-model="formData.if_returned" />
      </div>

      <label for="user_search">Search User</label>
      <input type="text" id="user_search" v-model="userSearch" placeholder="Search by name or phone" />

      <label for="user_id">User</label>
      <select id="user_id" v-model="formData.user_id" required>
        <option value="" disabled>Select a user</option>
        <option v-for="user in filteredUsers" :key="user.id" :value="user.id">
          {{ user.first_name }} {{ user.last_name }} {{ user.phone }}, ID: {{ user.id }}
        </option>
      </select>

      <label for="vehicle_search">Search Vehicle</label>
      <input type="text" id="vehicle_search" v-model="vehicleSearch" placeholder="Search by VIN" />

      <label for="car_vin">Car VIN</label>
      <select id="car_vin" v-model="formData.car_vin" @change="calculateTotalCost" required>
        <option value="" disabled>Select a vehicle</option>
        <option v-for="vehicle in filteredVehicles" :key="vehicle.vin" :value="vehicle.vin">
          {{ vehicle.make }} {{ vehicle.model }} VIN: {{ vehicle.vin }} - ${{ vehicle.price_per_day }}/day
        </option>
      </select>

      <p>Total Cost: ${{ formData.total_cost }}</p>

      <p class="validation-error" v-if="validationMessage">{{ validationMessage }}</p>

      <button class="button-primary" type="submit">Add Rental</button>
    </form>

    <p v-if="responseMessage">{{ responseMessage }}</p>
  </div>
</template>

<style scoped>
.validation-error {
  color: red;
  margin-top: 10px;
}
.c-formAddRental-row {
  display: flex; 
  align-items: center;
  gap: 10px;
  padding: 10
}
</style>