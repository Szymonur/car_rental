<script setup>
import { ref, onMounted, computed } from "vue";

const rentals = ref([]);
const users = ref([]);
const rentalSearch = ref("");
const responseMessage = ref("");

// Computed rentals with user details
const mergedRentals = computed(() => {
  return rentals.value.map(rental => {
    const user = users.value.find(u => u.id === rental.user_id);
    return {
      ...rental,
      user_name: user ? `${user.first_name} ${user.last_name}` : "Unknown",
      user_phone: user ? user.phone : "N/A",
    };
  });
});

const filteredRentals = computed(() => {
  if (!rentalSearch.value.trim()) return mergedRentals.value;
  return mergedRentals.value.filter(rental => {
    const rentalDetails = `${rental.user_name} ${rental.user_phone} ${rental.id} ${rental.car_vin}`.toLowerCase();
    return rentalDetails.includes(rentalSearch.value.toLowerCase());
  });
});

// Fetch rentals
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

// Fetch users
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

// Fetch data on mount
onMounted(() => {
  fetchRentals();
  fetchUsers();
});
</script>

<template>
  <div class="c-manageRentals">
    <h1>Manage Rentals</h1> <br>

    <label for="rental_search"> <h2>Search Rentals: </h2> </label>
    <input
      type="text"
      id="rental_search"
      v-model="rentalSearch"
      placeholder="Search by user name, phone, or rental ID"
    />

    <table >
      <thead>
        <tr>
          <th>Rental ID</th>
          <th>User</th>
          <th>Phone</th>
          <th>Car</th>
          <th>Rental Start</th>
          <th>Rental End</th>
          <th>Cost</th>
          <th>Is Accepted</th>
          <th>Menage</th>
          
        </tr>
      </thead>
      <tbody>
        <tr v-for="rental in filteredRentals" :key="rental.id">
          <td>{{ rental.id }}</td>
          <td>{{ rental.user_name }}</td>
          <td>{{ rental.user_phone }}</td>
          <td>{{ rental.car_vin }}</td>
          <td>{{ rental.rental_start }}</td>
          <td>{{ rental.rental_end }}</td>
          <td>{{ rental.total_cost }} $</td>
          <td>{{ rental.is_accepted }}</td>
          <td>
            <router-link :to="`/rentals/menage_rentals${rental.id}`">Manage</router-link>
          </td>
        </tr>
      </tbody>
    </table>

    <p v-if="responseMessage">{{ responseMessage }}</p>
  </div>
</template>