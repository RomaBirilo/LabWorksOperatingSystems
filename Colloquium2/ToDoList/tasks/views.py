import json
from json import JSONDecodeError

from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
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

def get_detail(task_id):
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

def delete(task_id):
    task = Task.objects.get(pk = task_id)
    task.delete()
    return JsonResponse({}, status = 204)

@csrf_exempt
def task_list(request):
    if request.method == 'GET':
        return get(request)
    if request.method == 'POST':
        return post(request)
    return JsonResponse({"error": "Method not allowed"}, status=405)

@csrf_exempt
def task_detail(request, task_id):
    if request.method == 'GET':
        return get_detail(task_id)
    if request.method == 'PUT':
        return put(request, task_id)
    if request.method == 'PATCH':
        return patch(request, task_id)
    if request.method == 'DELETE':
        return delete(task_id)
    return JsonResponse({"error": "Method not allowed"}, status=405)


