<script setup>
import TestApi from '@/components/TestApi.vue'
</script>
<script>
export default {
  data() {
    return {
      formData: {
        name: "",
        age: null,
      },
      responseMessage: "", // To display the response from the API
    };
  },
  methods: {
    async submitForm() {
      try {
        const response = await fetch("http://localhost:18080/add_user", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(this.formData), // Convert form data to JSON
        });

        if (response.ok) {
            const result = await response.json();
            this.responseMessage = result.message;
            
            // Reset formData after successful form submission
            this.formData = {
                first_name: "",
                last_name: "",
                email: "",
                phone: "",
                driving_exp_years: null,
            };
        } else {
          this.responseMessage = "Error: Failed to submit form.";
        }
      } catch (error) {
        console.error("Error:", error);
        this.responseMessage = "An error occurred while submitting the form.";
      }
    },
  },
};
</script>
<template>
    <div class="c-formAddUser">
        <div>
            <form @submit.prevent="submitForm">
                <h1>Add user</h1>
                <label for="first_name">First name</label>
                <input type="text" id="first_name" v-model="formData.first_name" required />

                <label for="last_name">Last name</label>
                <input type="text" id="last_name" v-model="formData.last_name" required />

                <label for="email">Email</label>
                <input type="email" id="email" v-model="formData.email" required />

                <label for="phone">Phone:</label>
                <input type="phone" id="last_name" v-model="formData.phone" required />

                <label for="driving_exp_years">Driving experience in years:</label>
                <input type="number" id="driving_exp_years" v-model="formData.driving_exp_years" required />
                
                <button class="button-primary" type="submit">Add</button>
                <!-- <button class="button-secondary" type="submit">Submit</button> -->
            </form>
            <p v-if="responseMessage">{{ responseMessage }}</p>
            </div>
            <div>
                <!-- ------------ DISPLAY USERS ------------ -->
                <!-- <TestApi /> -->
            </div>
        </div>
</template>

<style scoped>
form {
  display: flex;
  flex-direction: column;
  max-width: 300px;
}

label {
  margin-top: 10px;
}

button {
  margin-top: 20px;
}
</style>