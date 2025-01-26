<script setup>
import { ref, onMounted, computed } from "vue";

const rentals = ref([]);
const users = ref([]);
const rentalSearch = ref("");
const responseMessage = ref("");
const filterDate = ref("all"); // 'all', 'past', 'ongoing', 'future'
const filterStatus = ref("all"); // 'all', 'not_issued', 'issued', 'returned'

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

// Function to filter by rental date
const filterRentalsByDate = computed(() => {
  const currentDate = new Date();
  return mergedRentals.value.filter(rental => {
    const rentalStart = new Date(rental.rental_start);
    const rentalEnd = new Date(rental.rental_end);

    if (filterDate.value === "all") return true;

    if (filterDate.value === "past" && rentalEnd < currentDate) return true;
    if (filterDate.value === "ongoing" && rentalStart <= currentDate && rentalEnd >= currentDate) return true;
    if (filterDate.value === "future" && rentalStart > currentDate) return true;

    return false;
  });
});

// Function to filter by status
const filterRentalsByStatus = computed(() => {
  let rentalsList = filterRentalsByDate.value;

  if (filterStatus.value === "all") return rentalsList;
  if (filterStatus.value === "not_issued") {
    return rentalsList.filter(rental => !rental.if_issued);
  }
  if (filterStatus.value === "issued") {
    return rentalsList.filter(rental => rental.if_issued && !rental.if_returned);
  }
  if (filterStatus.value === "returned") {
    return rentalsList.filter(rental => rental.if_returned);
  }

  return rentalsList;
});

// Filter rentals based on search input
const filteredRentals = computed(() => {
  let rentalsList = filterRentalsByStatus.value;
  if (!rentalSearch.value.trim()) return rentalsList;

  return rentalsList.filter(rental => {
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

    <div class="c-menageRentals-filters">
      <label for="rental_search"><h2>Search Rentals:</h2></label>
      <input
        type="text"
        id="rental_search"
        v-model="rentalSearch"
        placeholder="Search by user name, phone, or rental ID"
      />

      <label for="date_filter"><h2>Filter by Rental Date:</h2></label>
      <select id="date_filter" v-model="filterDate">
        <option value="all">All</option>
        <option value="past">Past</option>
        <option value="ongoing">Current</option>
        <option value="future">Future</option>
      </select>

      <label for="status_filter"><h2>Filter by Status:</h2></label>
      <select id="status_filter" v-model="filterStatus">
        <option value="all">All</option>
        <option value="not_issued">Not Issued</option>
        <option value="issued">Issued</option>
        <option value="returned">Returned</option>
      </select>
    </div>
    <table>
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
          <th>If Paid</th>
          <th>Manage</th>
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
          <td>{{ rental.if_paid }}</td>
          <td>{{ rental.is_accepted }}</td>
          <td>
            <router-link :to="`/rentals/manage_rentals${rental.id}`">Manage</router-link>
          </td>
        </tr>
      </tbody>
    </table>

    <p v-if="responseMessage">{{ responseMessage }}</p>
  </div>
</template>

<style>
  .c-menageRentals-filters {
    display: flex;
    gap: 0.5rem;
  }
</style>
