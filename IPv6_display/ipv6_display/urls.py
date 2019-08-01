#!/usr/bin/env python
# -*- coding:utf-8 -*- 
# code_by: Tfflyer
#!/usr/bin/env python
# -*- coding:utf-8 -*-
# code_by: Tfflyer
from django.urls import path
from . import views
from django.conf import settings
from django.conf.urls.static import static
app_name = 'ipv6_display'
urlpatterns = [

    path('index/', views.index, name='index')]+ static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)