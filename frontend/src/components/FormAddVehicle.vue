<template>
    <div class="add-vehicle-form">
      <h2>Add a New Vehicle</h2>
      <form @submit.prevent="submitForm">
        
          <label for="vin">VIN:</label>
          <input v-model="vehicle.vin" type="text" id="vin" required />
  
          <label for="model">Model:</label>
          <input v-model="vehicle.model" type="text" id="model" required />
  
          <label for="brand">Brand:</label>
          <input v-model="vehicle.brand" type="text" id="brand" required />
  
          <label for="production_year">Production Year:</label>
          <input v-model="vehicle.production_year" type="number" id="production_year" required />
  
          <label for="mileage">Mileage (in km):</label>
          <input v-model="vehicle.mileage" type="number" id="mileage" required />
  
          <label for="color">Color:</label>
          <input v-model="vehicle.color" type="text" id="color" required />
  
          <label for="hp">Horsepower (HP):</label>
          <input v-model="vehicle.hp" type="number" id="hp" required />
        
          <label for="avg_fuel_usage">Average Fuel Usage (L/100 km):</label>
          <input v-model="vehicle.avg_fuel_usage" type="number" step="0.1" id="avg_fuel_usage" required />
  
          <label for="seats_number">Seats Number:</label>
          <input v-model="vehicle.seats_number" type="number" id="seats_number" required />
  
          <label for="doors_number">Doors Number:</label>
          <input v-model="vehicle.doors_number" type="number" id="doors_number" required />
  
          <label for="price_per_day">Price per Day (USD):</label>
          <input v-model="vehicle.price_per_day" type="number" id="price_per_day" required />
  
        <button class="button-primary" type="submit">Add Vehicle</button>
      </form>
  
      <div v-if="responseMessage" class="response-message">
        <p>{{ responseMessage }}</p>
      </div>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        vehicle: {
          vin: '',
          model: '',
          brand: '',
          production_year: '',
          mileage: '',
          color: '',
          hp: '',
          avg_fuel_usage: '',
          seats_number: '',
          doors_number: '',
          is_available: true,
          price_per_day: ''
        },
        responseMessage: ''
      };
    },
    methods: {
      async submitForm() {
        try {
          // Prepare form data as JSON
          const vehicleData = { ...this.vehicle };
  
          // Use fetch to send a POST request to the backend
          const response = await fetch('http://localhost:18080/add_vehicle', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json'
            },
            body: JSON.stringify(vehicleData)
          });
  
          // Check if the response is OK
          if (response.ok) {
            const responseData = await response.json();
            this.responseMessage = responseData.message || 'Vehicle added successfully!';
            this.resetForm();
          } else {
            throw new Error('Failed to add vehicle');
          }
        } catch (error) {
          this.responseMessage = `Error: ${error.message}`;
          console.error('Error adding vehicle:', error);
        }
      },
  
      resetForm() {
        this.vehicle = {
          vin: '',
          model: '',
          brand: '',
          production_year: '',
          mileage: '',
          color: '',
          hp: '',
          avg_fuel_usage: '',
          seats_number: '',
          doors_number: '',
          is_available: true,
          price_per_day: ''
        };
      }
    }
  };
  </script>
  
  <style scoped>
  </style>
  