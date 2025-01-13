<script setup>
</script>
<template>
    <div class="c-adminUsersViev">
    <nav class="nav-panel">
        <RouterLink to="/users/add_user">Add user</RouterLink>
        <RouterLink to="/users/delete_user">Delete user</RouterLink>
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
    .c-adminUsersViev{
        display: flex;
    }
</style>