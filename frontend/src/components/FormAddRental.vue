<script setup>
import { ref, onMounted, computed } from "vue";

const formData = ref({
  rental_id: null,
  rental_start: "",
  rental_end: "",
  is_accepted: false,
  car_vin: "",
  user_id: "",
});

const users = ref([]);
const vehicles = ref([]);
const responseMessage = ref("");
const userSearch = ref("");
const vehicleSearch = ref("");

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
  if (!vehicleSearch.value.trim()) return vehicles.value;
  return vehicles.value.filter(vehicle => 
    vehicle.vin.toLowerCase().includes(vehicleSearch.value.toLowerCase())
  );
});

// Fetch users and vehicles data
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

onMounted(() => {
  fetchUsers();
  fetchVehicles();
});

const submitForm = async () => {
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
      };
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
      <input type="date" id="rental_start" v-model="formData.rental_start" required />

      <label for="rental_end">Rental End</label>
      <input type="date" id="rental_end" v-model="formData.rental_end" required />

      <label for="is_accepted">Is Accepted</label>
      <input type="checkbox" id="is_accepted" v-model="formData.is_accepted" />

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
      <select id="car_vin" v-model="formData.car_vin" required>
        <option value="" disabled>Select a vehicle</option>
        <option v-for="vehicle in filteredVehicles" :key="vehicle.vin" :value="vehicle.vin">
          {{ vehicle.make }} {{ vehicle.model }} VIN: {{ vehicle.vin }}
        </option>
      </select>

      <button class="button-primary" type="submit">Add Rental</button>
    </form>

    <p v-if="responseMessage">{{ responseMessage }}</p>
  </div>
</template>
<style scoped>
form {
  display: flex;
  flex-direction: column;
  max-width: 400px;
  margin-right: 400px;
}

label {
  margin-top: 10px;
}

button {
  margin-top: 20px;
}
</style>
