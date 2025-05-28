
from django.shortcuts import render,redirect,HttpResponse
from .models import *
from django.contrib import messages
# Create your views here.
def index(request):
    return render(request,'index.html')
def home(request):
    return render(request,'home.html')

def register(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        email = request.POST.get('email')
        password = request.POST.get('password')
        Confirm_Password = request.POST.get('Confirm_Password')

        if password != Confirm_Password:
            return HttpResponse("<script>alert('Password do not match!'); window.history.back();</script>")
        if userregister.objects.filter(username=username).exists():
            return HttpResponse("<script>alert('Username already exists!'); window.history.back();</script>")
        if userregister.objects.filter(email=email).exists():
            return HttpResponse("<script>alert('Email alreday registered!'); window.history.back();</script>")
        user=userregister(username=username,email=email,password=password)
        user.save()
        return HttpResponse("<script>alert('Registration Successful! Please login.'); window.location.href='/login/';</script>")
    return render(request,'register.html')

def login(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        try:
           user = userregister.objects.get(username=username,password=password)
           request.session['username'] = user.username
           return redirect('home')
        except user.DoesNotExist:
            return render(request,'login.html',{'error':'Invalid email or password.'})
    return render(request,'login.html')

def ben(request):
    return render(request,'salmu.html')


def profile(request):
    username=request.session.get('username')
    form=userregister.objects.filter(username=username)
    return render(request,'profile.html',{'form':form})

def editprofile(request):
    username=request.session.get('username')
    user=userregister.objects.get(username=username)
    
    if request.method == 'POST' :
        username = request.POST.get('username')
        email = request.POST.get('email')
        password = request.POST.get('password')

        user.username = username
        user.email = email
        user.password = password
        user.save()
        messages.success(request, 'pofile update successfully!')
        return redirect('profile')
    return render(request, 'editprofile.html', {'user':user})

def logout(request):
    request.session.flush()
    return redirect('index')