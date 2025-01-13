<script setup>
</script>
<template>
    <div class="c-adminCarsViev">
    <nav class="nav-panel">
        <RouterLink to="/cars/add_vehicle">Add vehicle</RouterLink>
        <RouterLink to="/cars/delete_vehicle">Delete vehicle</RouterLink>

    </nav>
    <div class="router-view">
      <router-view></router-view>
    </div>

    <div>
        
    </div>
    </div>
</template>

<style scoped>
    .c-adminCarsViev{
        display: flex;
    }
</style>