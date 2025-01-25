import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import AdminUsersView from '../views/AdminUsersView.vue'
import AdminCarsView from '../views/AdminCarsView.vue'
import AdminRentalsView from '../views/AdminRentalsView.vue'

import FormDeleteUser from '../components/FormDeleteUser.vue'
import FormAddUser from '../components/FromAddUser.vue'
import FormAddVehicle from '../components/FormAddVehicle.vue'
import FormDeleteVehicle from '../components/FormDeleteVehicle.vue'
import FormAddRental from '../components/FormAddRental.vue'
import MenageRentals from '../components/MenageRentals.vue'
import EditRental from '../components/EditRental.vue'




 
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView,
    },    
    {
      path: '/users',
      name: 'users',
      component: AdminUsersView,
      children: [
        {
          path: 'delete_user',
          name: 'delete_user',
          component: FormDeleteUser,
        },
        {
          path: 'add_user',
          name: 'add_user',
          component: FormAddUser,
        },
      ]
    },
    {
      path: '/cars',
      name: 'cars',
      component: AdminCarsView,
      children: [
        {
          path: 'add_vehicle',
          name: 'add_vehicle',
          component: FormAddVehicle,
        },
        {
          path: 'delete_vehicle',
          name: 'delete_vehicle',
          component: FormDeleteVehicle,
        },
      ]
    },
    {
      path: '/rentals',
      name: 'rentals',
      component: AdminRentalsView,
      children: [
        {
          path: 'add_rental',
          name: 'add_rental',
          component: FormAddRental,
        },
        {
          path: 'menage_rentals',
          name: 'menage_rentals',
          component: MenageRentals
        },
        {
          path: 'menage_rentals:id',
          name: 'menage_rentals_id',
          component: EditRental
        },
      ]
    }
    
    // {
    //   path: '/about',
    //   name: 'about',
    //   // route level code-splitting
    //   // this generates a separate chunk (About.[hash].js) for this route
    //   // which is lazy-loaded when the route is visited.
    //   component: () => import('../views/AboutView.vue'),
    // },
  ],
})

export default router
