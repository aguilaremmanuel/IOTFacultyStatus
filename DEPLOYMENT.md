# Deployment Guide

## Local Development

### Setup

1. Create and activate virtual environment:
   ```bash
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

3. The `.env` file is already configured for local development.

4. Run migrations:
   ```bash
   python manage.py migrate
   ```

5. Start the development server:
   ```bash
   python manage.py runserver
   ```

6. Access the site at: http://127.0.0.1:8000

---

## PythonAnywhere Deployment

### Step 1: Upload Your Code

Option A - Via Git:
```bash
git clone https://github.com/yourusername/IOT_WEBSITE.git
```

Option B - Upload ZIP file through PythonAnywhere Files tab.

### Step 2: Create Virtual Environment

In a PythonAnywhere Bash console:
```bash
cd ~/IOT_WEBSITE
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

### Step 3: Configure Environment Variables

Create a `.env` file in your project directory:
```bash
nano ~/IOT_WEBSITE/.env
```

Add the following (replace with your values):
```
SECRET_KEY=your-new-secret-key-here
DEBUG=False
ALLOWED_HOSTS=yourusername.pythonanywhere.com
```

Generate a new secret key:
```bash
python -c "from django.core.management.utils import get_random_secret_key; print(get_random_secret_key())"
```

### Step 4: Collect Static Files

```bash
cd ~/IOT_WEBSITE
source venv/bin/activate
python manage.py collectstatic
```

### Step 5: Run Migrations

```bash
python manage.py migrate
```

### Step 6: Create Superuser (optional)

```bash
python manage.py createsuperuser
```

### Step 7: Configure Web App

1. Go to **Web** tab in PythonAnywhere
2. Click **Add a new web app**
3. Select **Manual configuration** (not Django)
4. Choose Python version (3.10 or higher)

#### WSGI Configuration

Edit the WSGI configuration file (click the link in the Web tab):

```python
import os
import sys

# Add your project directory to the path
project_home = '/home/yourusername/IOT_WEBSITE'
if project_home not in sys.path:
    sys.path.insert(0, project_home)

# Set the settings module
os.environ['DJANGO_SETTINGS_MODULE'] = 'sfsi_project.settings'

# Load the Django WSGI application
from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()
```

#### Virtual Environment

In the Web tab, set the **Virtualenv** path to:
```
/home/yourusername/IOT_WEBSITE/venv
```

#### Static Files

In the Web tab, add static files mapping:
- URL: `/static/`
- Directory: `/home/yourusername/IOT_WEBSITE/staticfiles`

### Step 8: Reload Web App

Click the **Reload** button in the Web tab.

---

## Environment Variables Reference

| Variable | Local | Production |
|----------|-------|------------|
| `SECRET_KEY` | Use default | Generate new key |
| `DEBUG` | `True` | `False` |
| `ALLOWED_HOSTS` | `localhost,127.0.0.1` | `yourusername.pythonanywhere.com` |

---

## Troubleshooting

### Error: ModuleNotFoundError
Make sure you activated the virtual environment and installed all dependencies.

### Static files not loading
1. Run `python manage.py collectstatic`
2. Check static files mapping in PythonAnywhere Web tab

### 500 Server Error
1. Check the error log in PythonAnywhere Web tab
2. Ensure `.env` file exists with correct values
3. Verify `DEBUG=False` and `ALLOWED_HOSTS` is set correctly
