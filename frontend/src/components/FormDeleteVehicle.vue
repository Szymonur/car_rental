<script setup>
import { ref, onMounted } from 'vue';

// Reactive variables for data, loading, and error states
const vehicles = ref([]);
const loading = ref(true);
const error = ref(null);

const fetchVehicles = async () => {
  try {
    const response = await fetch('http://localhost:18080/vehicles'); // Fetch data from the server
    if (!response.ok) {
      throw new Error(`Error: ${response.statusText}`);
    }
    vehicles.value = await response.json(); // Parse the JSON response and update `vehicles`
  } catch (err) {
    error.value = err.message; // Handle any errors
  } finally {
    loading.value = false; // Set loading to false after fetch completes
  }
};

const deleteVehicle = async (vin) => {
  try {
    const response = await fetch(`http://localhost:18080/vehicle/${vin}`, {
      method: 'DELETE', // Specify DELETE method
    });
    if (!response.ok) {
      throw new Error(`Failed to delete vehicle with VIN ${vin}`);
    }
    await fetchVehicles(); // Refresh the vehicle list after successful deletion
    alert(`Vehicle with VIN ${vin} deleted successfully.`);
  } catch (err) {
    alert(`Error: ${err.message}`); // Handle any errors
  }
};


// Fetch vehicle data when the component is mounted
onMounted(() => {
  fetchVehicles();
});
</script>

<template>
  <div class="c-vehicleManagement">
    <div v-if="loading">Loading...</div>
    <div v-else-if="error">Error: {{ error }}</div>
    <div v-else>
      <h2>Vehicle List</h2>
      <table border="1">
        <thead>
          <tr>
            <th>VIN</th>
            <th>Model</th>
            <th>Brand</th>
            <th>Production Year</th>
            <th>Mileage</th>
            <th>Color</th>
            <th>Horsepower</th>
            <th>Avg Fuel Usage</th>
            <th>Seats</th>
            <th>Doors</th>
            <th>Available</th>
            <th>Price/Day</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="vehicle in vehicles" :key="vehicle.vin">
            <td>{{ vehicle.vin }}</td>
            <td>{{ vehicle.model }}</td>
            <td>{{ vehicle.brand }}</td>
            <td>{{ vehicle.production_year }}</td>
            <td>{{ vehicle.mileage }}</td>
            <td>{{ vehicle.color }}</td>
            <td>{{ vehicle.hp }}</td>
            <td>{{ vehicle.avg_fuel_usage }}</td>
            <td>{{ vehicle.seats_number }}</td>
            <td>{{ vehicle.doors_number }}</td>
            <td>{{ vehicle.is_available ? 'Yes' : 'No' }}</td>
            <td>{{ vehicle.price_per_day }}</td>
            <td>
              <button @click="deleteVehicle(vehicle.vin)">Delete</button>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>
