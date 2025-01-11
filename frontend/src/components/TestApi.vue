<script setup>
import { ref, onMounted } from 'vue';

// Create a reactive variable to store data and loading state
const data = ref(null);
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

// Fetch data when the component is mounted
onMounted(() => {
  fetchData();
});
</script>

<template>
  <div>

    <div v-if="loading">Loading...</div>
    <div v-else-if="error">Error: {{ error }}</div>
    <div v-else>
      <pre>{{ data }}</pre>
    </div>
  </div>
</template>
