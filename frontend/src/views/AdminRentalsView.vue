<script setup>
</script>
<template>
    <div class="c-adminRentalsView">
    <nav class="nav-panel">
        <RouterLink to="/rentals/add_rental">Create rental</RouterLink>
        <RouterLink to="/rentals/manage_rentals">Manage rentals</RouterLink>
        <!-- <RouterLink to="/about">About</RouterLink> -->
    </nav>
    <div class="router-view">
      <router-view></router-view>
    </div>

    <div>
        
    </div>
    </div>
</template>

<style scoped>
    .c-adminRentalsView{
        display: flex;
    }
</style>