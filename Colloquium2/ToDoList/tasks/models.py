from django.db import models

class Task(models.Model):
    STATUS_TODO = 'todo'
    STATUS_IN_PROGRESS = 'in_progress'
    STATUS_DONE = 'done'

    STATUS_CHOICES = [
        (STATUS_TODO,'To Do'),
        (STATUS_IN_PROGRESS,'In Progress'),
        (STATUS_DONE,'Done')
    ]

    title = models.CharField(
        max_length = 20,
        verbose_name = "Название"
    )

    description = models.TextField(
        blank = True,
        verbose_name = "Описание"
    )

    status = models.CharField(
        max_length = 20,
        choices = STATUS_CHOICES,
        default = STATUS_TODO,
        verbose_name = 'Статус'
    )

    def __str__(self):
        return self.title

    def to_dict(self):
        return{
            'id':self.id,
            'title':self.title,
            'description': self.description,
            'status': self.status
        }