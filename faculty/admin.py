from django.contrib import admin
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.models import User
from django.utils.html import format_html
from .models import Teacher, Department


# Customize admin site header
admin.site.site_header = "SFSI Faculty Management System"
admin.site.site_title = "SFSI Admin"
admin.site.index_title = "System Administration"


@admin.register(Department)
class DepartmentAdmin(admin.ModelAdmin):
    list_display = ('code', 'name', 'teacher_count', 'created_at')
    search_fields = ('name', 'code')
    ordering = ('name',)

    def teacher_count(self, obj):
        return obj.teachers.count()
    teacher_count.short_description = 'Faculty Members'


class TeacherInline(admin.StackedInline):
    model = Teacher
    can_delete = False
    verbose_name_plural = 'Faculty Profile'
    fields = ('department', 'position', 'status')


class CustomUserAdmin(UserAdmin):
    inlines = (TeacherInline,)
    list_display = ('username', 'email', 'first_name', 'last_name', 'get_department', 'get_status_display', 'is_staff')
    list_filter = ('is_staff', 'is_active', 'teacher__department', 'teacher__status')

    def get_department(self, obj):
        if hasattr(obj, 'teacher') and obj.teacher.department:
            return obj.teacher.department.code
        return '-'
    get_department.short_description = 'Department'

    def get_status_display(self, obj):
        if hasattr(obj, 'teacher'):
            status = obj.teacher.status
            colors = {
                'available': '#28a745',
                'busy': '#ffc107',
                'not_present': '#dc3545',
            }
            color = colors.get(status, '#6c757d')
            return format_html(
                '<span style="background-color: {}; color: white; padding: 3px 10px; border-radius: 3px;">{}</span>',
                color,
                obj.teacher.get_status_display()
            )
        return '-'
    get_status_display.short_description = 'Status'


# Unregister the original User admin and register the custom one
admin.site.unregister(User)
admin.site.register(User, CustomUserAdmin)


@admin.register(Teacher)
class TeacherAdmin(admin.ModelAdmin):
    list_display = ('full_name', 'department', 'position', 'status_badge', 'last_updated')
    list_filter = ('status', 'department')
    search_fields = ('user__username', 'user__first_name', 'user__last_name', 'department__name')
    list_editable = ('department', 'position')
    list_per_page = 25
    date_hierarchy = 'last_updated'

    fieldsets = (
        ('User Information', {
            'fields': ('user',)
        }),
        ('Faculty Details', {
            'fields': ('department', 'position')
        }),
        ('Status', {
            'fields': ('status',),
            'description': 'Update faculty availability status'
        }),
    )

    def full_name(self, obj):
        return obj.user.get_full_name() or obj.user.username
    full_name.short_description = 'Name'
    full_name.admin_order_field = 'user__last_name'

    def status_badge(self, obj):
        colors = {
            'available': '#28a745',
            'busy': '#ffc107',
            'not_present': '#dc3545',
        }
        color = colors.get(obj.status, '#6c757d')
        return format_html(
            '<span style="background-color: {}; color: white; padding: 3px 10px; border-radius: 3px;">{}</span>',
            color,
            obj.get_status_display()
        )
    status_badge.short_description = 'Status'

    actions = ['mark_available', 'mark_busy', 'mark_not_present']

    @admin.action(description='Mark selected as Available')
    def mark_available(self, request, queryset):
        updated = queryset.update(status='available')
        self.message_user(request, f'{updated} faculty member(s) marked as available.')

    @admin.action(description='Mark selected as Busy')
    def mark_busy(self, request, queryset):
        updated = queryset.update(status='busy')
        self.message_user(request, f'{updated} faculty member(s) marked as busy.')

    @admin.action(description='Mark selected as Not Present')
    def mark_not_present(self, request, queryset):
        updated = queryset.update(status='not_present')
        self.message_user(request, f'{updated} faculty member(s) marked as not present.')
