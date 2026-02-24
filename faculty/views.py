from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib import messages
from django.db import models
from django.contrib.auth.models import User
from .models import Teacher, Department


def public_status(request):
    """Public view for students to see all teacher statuses"""
    teachers = Teacher.objects.select_related('user').all()

    # Search functionality
    search_query = request.GET.get('search', '')
    if search_query:
        teachers = teachers.filter(
            models.Q(user__first_name__icontains=search_query) |
            models.Q(user__last_name__icontains=search_query) |
            models.Q(department__name__icontains=search_query)
        )

    return render(request, 'faculty/public_status.html', {
        'teachers': teachers,
        'search_query': search_query,
    })


def tv_display(request):
    """TV-optimized fullscreen display for faculty status"""
    teachers = Teacher.objects.select_related('user').all()
    return render(request, 'faculty/tv_display.html', {
        'teachers': teachers,
    })


def teacher_login(request):
    """Login view for teachers"""
    if request.user.is_authenticated:
        return redirect('dashboard')

    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        user = authenticate(request, username=username, password=password)

        if user is not None:
            # Check if user has a teacher profile
            if hasattr(user, 'teacher'):
                login(request, user)
                messages.success(request, 'Welcome back!')
                return redirect('dashboard')
            else:
                messages.error(request, 'You do not have a teacher account.')
        else:
            messages.error(request, 'Invalid username or password.')

    return render(request, 'faculty/login.html')


def teacher_logout(request):
    """Logout view"""
    logout(request)
    messages.success(request, 'You have been logged out.')
    return redirect('public_status')


def teacher_register(request):
    """Registration view for new teachers"""
    if request.user.is_authenticated:
        return redirect('dashboard')

    departments = Department.objects.all()

    if request.method == 'POST':
        username = request.POST.get('username', '').strip()
        password = request.POST.get('password', '')
        confirm_password = request.POST.get('confirm_password', '')
        first_name = request.POST.get('first_name', '').strip()
        last_name = request.POST.get('last_name', '').strip()
        email = request.POST.get('email', '').strip()
        department_id = request.POST.get('department', '').strip()
        position = request.POST.get('position', '').strip()

        # Validation
        errors = []
        if not username:
            errors.append('Username is required.')
        elif User.objects.filter(username=username).exists():
            errors.append('Username already exists.')

        if not password:
            errors.append('Password is required.')
        elif len(password) < 6:
            errors.append('Password must be at least 6 characters.')
        elif password != confirm_password:
            errors.append('Passwords do not match.')

        if not first_name:
            errors.append('First name is required.')
        if not last_name:
            errors.append('Last name is required.')

        if not email:
            errors.append('Email is required.')
        elif User.objects.filter(email=email).exists():
            errors.append('Email already registered.')

        if not department_id:
            errors.append('Department is required.')

        if not position:
            errors.append('Position is required.')

        # Get department instance if provided
        department = None
        if department_id:
            try:
                department = Department.objects.get(id=department_id)
            except Department.DoesNotExist:
                errors.append('Invalid department selected.')

        if errors:
            for error in errors:
                messages.error(request, error)
        else:
            # Create user and teacher profile
            user = User.objects.create_user(
                username=username,
                password=password,
                first_name=first_name,
                last_name=last_name,
                email=email
            )
            Teacher.objects.create(
                user=user,
                department=department,
                position=position
            )
            messages.success(request, 'Registration successful! Please log in.')
            return redirect('login')

    return render(request, 'faculty/register.html', {'departments': departments})


@login_required(login_url='login')
def dashboard(request):
    """Teacher dashboard to update status"""
    # Check if user has a teacher profile
    try:
        teacher = request.user.teacher
    except Teacher.DoesNotExist:
        messages.error(request, 'You do not have a teacher profile. Please contact admin or register as a teacher.')
        return redirect('public_status')

    if request.method == 'POST':
        new_status = request.POST.get('status')
        if new_status in ['available', 'busy', 'not_present']:
            teacher.status = new_status
            teacher.save()
            messages.success(request, f'Status updated to {teacher.get_status_display()}')
            return redirect('dashboard')

    return render(request, 'faculty/dashboard.html', {
        'teacher': teacher,
    })


# API Endpoints for ESP32
@csrf_exempt
def api_all_status(request):
    """API endpoint to get all teacher statuses (for ESP32)"""
    teachers = Teacher.objects.select_related('user', 'department').all()
    data = {
        'teachers': [
            {
                'id': t.id,
                'name': t.user.get_full_name() or t.user.username,
                'department': t.department.name if t.department else '',
                'status': t.status,
                'status_display': t.get_status_display(),
                'color': t.get_status_color(),
            }
            for t in teachers
        ]
    }
    return JsonResponse(data)


@csrf_exempt
def api_teacher_status(request, teacher_id):
    """API endpoint to get single teacher status (for ESP32)"""
    try:
        teacher = Teacher.objects.select_related('user', 'department').get(id=teacher_id)
        data = {
            'id': teacher.id,
            'name': teacher.user.get_full_name() or teacher.user.username,
            'department': teacher.department.name if teacher.department else '',
            'status': teacher.status,
            'status_display': teacher.get_status_display(),
            'color': teacher.get_status_color(),
            'last_updated': teacher.last_updated.isoformat(),
        }
        return JsonResponse(data)
    except Teacher.DoesNotExist:
        return JsonResponse({'error': 'Teacher not found'}, status=404)
