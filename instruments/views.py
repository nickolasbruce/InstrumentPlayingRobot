from django.shortcuts import render
from django.shortcuts import redirect
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
import json
import urllib3

# Create your views here.
def index(request):
	return render(request, 'index.html')

@csrf_exempt
def jsonhandler(request):
	if request.method == 'POST':

		# Obtain note from browser
		note = json.load(request)

		# Write to USB through serial
		print(note)
		if len(note['port']) > 0:
			url = "http://" + note['port'] + "/" \
											+ note['count'] + "/" \
											+ note['key'] + "/"
			print(url)
			http = urllib3.PoolManager()
			r = http.request('GET', url)

		# Pass data to view
		context = {
			'status': "OK"
		}
		return HttpResponse(context)

	elif request.method == 'GET':
		return redirect('/play')