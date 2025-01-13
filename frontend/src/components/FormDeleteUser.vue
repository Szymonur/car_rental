<script setup>
import { ref, onMounted } from 'vue';

// Reactive variables for data, loading, and error states
const data = ref([]);
const loading = ref(true);
const error = ref(null);

// Function to fetch data
const fetchData = async () => {
  try {
    const response = await fetch('http://localhost:18080/users');
    if (!response.ok) {
      throw new Error(`Error: ${response.statusText}`);
    }
    data.value = await response.json();
  } catch (err) {
    error.value = err.message;
  } finally {
    loading.value = false;
  }
};

// Function to delete a user by ID
const deleteUser = async (id) => {
  try {
    const response = await fetch(`http://localhost:18080/users/${id}`, {
      method: 'DELETE',
    });
    if (!response.ok) {
      throw new Error(`Failed to delete user with ID ${id}`);
    }
    // Refresh the data after deletion
    await fetchData();
    alert(`User with ID ${id} deleted successfully.`);
  } catch (err) {
    alert(`Error: ${err.message}`);
  }
};

// Fetch data when the component is mounted
onMounted(() => {
  fetchData();
});
</script>

<template>
  <div class="c-deleteUserForm">
    <div v-if="loading">Loading...</div>
    <div v-else-if="error">Error: {{ error }}</div>
    <div v-else>
      <h2>User List</h2>
      <table border="1">
        <thead>
          <tr>
            <th>ID</th>
            <th>First Name</th>
            <th>Last Name</th>
            <th>Email</th>
            <th>Phone</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="user in data" :key="user.id">
            <td>{{ user.id }}</td>
            <td>{{ user.first_name }}</td>
            <td>{{ user.last_name }}</td>
            <td>{{ user.email }}</td>
            <td>{{ user.phone }}</td>
            <td>
              <button @click="deleteUser(user.id)">Delete</button>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>
