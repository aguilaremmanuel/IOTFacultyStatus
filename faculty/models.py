from django.db import models
from django.contrib.auth.models import User


class Department(models.Model):
    name = models.CharField(max_length=100, unique=True)
    code = models.CharField(max_length=20, unique=True, help_text="Short code for the department (e.g., CS, IT, EE)")
    description = models.TextField(blank=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.code} - {self.name}"

    class Meta:
        ordering = ['name']


class Teacher(models.Model):
    STATUS_CHOICES = [
        ('available', 'Available'),
        ('busy', 'Busy'),
        ('not_present', 'Not Present'),
    ]

    user = models.OneToOneField(User, on_delete=models.CASCADE)
    department = models.ForeignKey(Department, on_delete=models.SET_NULL, null=True, blank=True, related_name='teachers')
    position = models.CharField(max_length=100, blank=True)
    status = models.CharField(max_length=20, choices=STATUS_CHOICES, default='not_present')
    last_updated = models.DateTimeField(auto_now=True)

    def __str__(self):
        return f"{self.user.get_full_name() or self.user.username}"

    def get_status_color(self):
        """Returns color for status display"""
        colors = {
            'available': '#28a745',      # Green
            'busy': '#ffc107',           # Yellow
            'not_present': '#dc3545',    # Red
        }
        return colors.get(self.status, '#6c757d')

    class Meta:
        ordering = ['user__last_name', 'user__first_name']
