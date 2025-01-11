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
    <div class="c-adminPanel">
        <div>
          
                <form @submit.prevent="submitForm">
                    <h1>Submit Form</h1>
                    <label for="name">Name:</label>
                    <input type="text" id="name" v-model="formData.name" required />
                    <label for="name">Name:</label>
                    <input type="text" id="name" v-model="formData.name" required />
                    <button class="button-primary" type="submit">Submit</button>
                    <button class="button-secondary" type="submit">Submit</button>
                </form>
                <p v-if="responseMessage">{{ responseMessage }}</p>
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