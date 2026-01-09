from django.urls import path
from rest_framework.authtoken.views import obtain_auth_token
from . import views

urlpatterns = [
    path('tasks/', views.task_list),
    path('tasks/<int:task_id>/', views.task_detail),
    path('list/', views.tasks_list, name='tasks_list'),
    path('', views.login, name = 'login'),
    path('api/token/', obtain_auth_token),
]
