<script setup>
import { RouterLink, RouterView } from 'vue-router'
</script>

<template>
    <nav>
      <RouterLink to="/rentals/add_rental">Rentals</RouterLink>
      <RouterLink to="/users/add_user">Users</RouterLink>
      <RouterLink to="/cars/add_vehicle">Cars</RouterLink>
      <!-- <RouterLink to="/about">About</RouterLink> -->
    </nav>
    <router-view></router-view>

</template>

<style scoped>

</style>
