from django.urls import path
from . import views

urlpatterns = [
    # Public views
    path('', views.public_status, name='public_status'),
    path('tv/', views.tv_display, name='tv_display'),

    # Teacher authentication
    path('login/', views.teacher_login, name='login'),
    path('register/', views.teacher_register, name='register'),
    path('logout/', views.teacher_logout, name='logout'),

    # Teacher dashboard
    path('dashboard/', views.dashboard, name='dashboard'),

    # API endpoints for ESP32
    path('api/status/', views.api_all_status, name='api_all_status'),
    path('api/status/<int:teacher_id>/', views.api_teacher_status, name='api_teacher_status'),
]
