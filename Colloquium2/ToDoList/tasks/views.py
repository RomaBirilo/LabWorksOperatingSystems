import json
from json import JSONDecodeError
from django.shortcuts import render, redirect, get_object_or_404
from django.contrib.auth import authenticate
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse
from rest_framework.decorators import api_view, permission_classes, authentication_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import TokenAuthentication
from rest_framework.authtoken.models import Token
import requests

from .models import Task


def get(reuqest):
    tasks = Task.objects.all()
    data = [task.to_dict() for task in tasks]
    return JsonResponse(data, safe = False, status = 200)

def post(request):
    try:
        body = json.loads(request.body)
    except json.JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"}, status=400)
    title = body.get('title')
    description = body.get('description','')
    status = body.get('status', Task.STATUS_TODO)

    if not title:
        return JsonResponse({"error": "Title is required"}, status=400)
    if status not in dict(Task.STATUS_CHOICES):
        return JsonResponse({"error": "Invalid status"}, status=400)

    task = Task.objects.create(
        title = title,
        description = description,
        status = status
    )
    return JsonResponse(task.to_dict(), status=201)

def get_detail(request, task_id):
    try:
        task = Task.objects.get(pk = task_id)
    except Task.DoesNotExist:
        return JsonResponse({"error": "Task not found"}, status=404)
    return JsonResponse(task.to_dict(), status = 200)

def put(request, task_id):
    try:
        body = json.loads(request.body)
    except json.JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"}, status=400)

    title = body.get('title')
    description = body.get('description', '')
    status = body.get('status')

    if not title:
        return JsonResponse({"error": "Title is required"}, status=400)
    if status not in dict(Task.STATUS_CHOICES):
        return JsonResponse({"error": "Invalid status"}, status=400)

    task = Task.objects.get(pk = task_id)

    task.title = title
    task.description = description
    task.status = status
    task.save()

    return JsonResponse(task.to_dict(), status = 200)

def patch(request, task_id):
    try:
        body = json.loads(request.body)
    except JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"},status = 400)
    task = Task.objects.get(pk = task_id)

    if 'title' in body:
        task.title = body.get('title')
    if 'description' in body:
        task.description = body.get('description')
    if 'status' in body:
        if body["status"] not in dict(Task.STATUS_CHOICES):
            return JsonResponse({"error": "Invalid status"}, status=400)
        task.status = body.get('status')
    task.save()
    return JsonResponse(task.to_dict(), status = 200)

def delete(request, task_id):
    task = Task.objects.get(pk = task_id)
    task.delete()
    return JsonResponse({}, status = 204)

@api_view(['GET', 'POST'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def task_list(request):
    if request.method == 'GET':
        return get(request)
    if request.method == 'POST':
        return post(request)
    return JsonResponse({"error": "Method not allowed"}, status=405)

@api_view(['GET', 'PUT', 'PATCH', 'DELETE'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def task_detail(request, task_id):
    if request.method == 'GET':
        return get_detail(request,task_id)
    if request.method == 'PUT':
        return put(request, task_id)
    if request.method == 'PATCH':
        return patch(request, task_id)
    if request.method == 'DELETE':
        return delete(request, task_id)
    return JsonResponse({"error": "Method not allowed"}, status=405)


def login(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')

        user = authenticate(request, username=username, password=password)
        if user is not None:
            token, created = Token.objects.get_or_create(user=user)
            request.session['auth_token'] = token.key
            request.session['username'] = user.username
            return redirect('list/')
        else:
            return render(request, 'tasks/login.html', {'error': 'Invalid username or password'})

    return render(request, 'tasks/login.html')


@login_required
def tasks_list(request):
    token = request.session.get('auth_token')
    if not token:
        return redirect('login')

    api_url = "http://127.0.0.1:8000/tasks/"
    headers = {'Authorization': f'Token {token}', 'Content-Type': 'application/json'}

    try:
        response = requests.get(api_url, headers=headers)
        response.raise_for_status()
        tasks = response.json()
    except requests.exceptions.RequestException as e:
        tasks = []
        return render(request, 'tasks/tasks_list.html', {'error': f'API error: {e}', 'tasks': tasks})

    return render(request, 'tasks/tasks_list.html', {'tasks': tasks})
