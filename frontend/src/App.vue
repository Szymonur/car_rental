<script setup>
import { RouterLink, RouterView } from 'vue-router'
</script>

<template>
    <nav>
      <RouterLink to="/">Rentals</RouterLink>
      <RouterLink to="/users">Users</RouterLink>
      <RouterLink to="/cars">Cars</RouterLink>
      <!-- <RouterLink to="/about">About</RouterLink> -->
    </nav>
    <router-view></router-view>

</template>

<style scoped>

</style>
