#
# UVic SENG 265, Summer 2019, Assignment #3
#
# Student work is to appear in this module.
#
import sys
import argparse
import time
from datetime import datetime, timedelta
from datetime import datetime
import operator
import re


class Calprint:

    def __init__(self, filename):
        self.hello = "Alive"
        self.filename = filename

    
    def get_start_end(self,dt):
        date = re.search(r'\d{4}-\d{2}-\d{2}',str(dt)).group(0)
        date = re.sub(r'-','',date)
        return date
        

    def find_repeated_date(self,start,end,until):
        repeated_list = []
        while start<until:
            start_incrent = self.date_decrent(start)   
            start_format =  start_incrent .strftime("%Y%m%d%H%M%S")

            if start_format < until:
                repeated_list.append(start_format)

            end_decrent = self.date_decrent(end)
            end_format =  end_decrent.strftime("%Y%m%d%H%M%S")

            if end_format<until:
                repeated_list.append(end_format)

            start = start_format
            end = end_format

        return repeated_list


    def catch_date(self,location,summary,start):
        location = sorted(location.items(),key=operator.itemgetter(0))
        summary = sorted(summary.items(),key=operator.itemgetter(0))   

        location = dict(location)
        summary = dict(summary)	
        date_period = { k: summary[k]+" "+"["+location[k]+"]" for k in location}
        
        select_period = {}
        for k in date_period:
            matchobj = re.match('(\d{8}).*',k)
            if matchobj:
                single_day = matchobj.group(0)
                if single_day[0:8] == start[0:8]:
                    select_period.update({ k : date_period[k]})
        return select_period


    def get_time_dictionary(self,date_list,start):
        secondlist = []
        c = 0
        location = {}
        summary = {}
        flist = []
        flist1 = []       
        for x in date_list:
            if(re.search(r'DTSTART.*',x)):
                x = re.sub(r'DTSTART:','',x)
                x = re.sub(r'T','',x)
                secondlist.append(x)	
            if(re.search(r'DTEND.*',x)):
                x = re.sub(r'DTEND:','',x)
                x = re.sub(r'T','',x)
                secondlist.append(x)
            if(re.search(r'UNTIL.*',x)):
                c = len(secondlist)
                dt = secondlist[c-2]
                de = secondlist[c-1]
                repeatedlist = self.find_repeated_date(dt,de,x[32:40]+x[41:47])
                for x in repeatedlist:
                    secondlist.append(x)	
            if (re.search(r'LOCATION.*',x)):
                location.update ({ i : x[9:] for i in secondlist})
                flist.append(location)
                location = {}			
            if (re.search(r'SUMMARY.*',x)):
                summary.update ({ i : x[8:] for i in secondlist})	
                flist1.append(summary)
                summary = {}
                secondlist = []	
        location = self.merge_dictionary(flist)
        summary = self.merge_dictionary(flist1)
        final_dic = self.catch_date(location,summary,start)
        return final_dic

    def merge_dictionary(self,list_dictionary):
        onedic = {}
        for d in list_dictionary:
            for k in d:
                if k not in onedic:
                    onedic.update({k:d[k]})
                else:
                    onedic[k] = onedic[k]+d[k]+"same_time_case"
        return onedic    

    def print_format(self,dic):
        format_string = ''
        key_list = list(dic)
        day = datetime.strptime(key_list[0], "%Y%m%d%H%M%S")
        day = day.strftime('%B %d, %Y(%a)')
        format_string = format_string + day
        format_string = format_string + "\n"+("-"*len(day))
        i = 0
        while True:
            x = key_list[i]
            y = key_list[i+1]
            DS = datetime.strptime(x, "%Y%m%d%H%M%S") 
            DT = datetime.strptime(y, "%Y%m%d%H%M%S")  		
            DS = DS.strftime('%I:%M %p')
            DT = DT.strftime('%I:%M %p')
            for k in dic:
                if k == x:
                    m = dic[k]
            if "same_time_case" in m:
                format_string = format_string +"\n"+(DS + " to " + DT + ":" + m[0:8] + m[40:48]+"]")           		
                format_string = format_string +"\n"+(DS + " to " + DT + ":" + m[8:25] +"["+ m[48:56]+"]") 		
            else:            		
                format_string = format_string +"\n"+(DS + " to " + DT + ":" + m)
            i = i + 2
            if i == len(key_list):
                break
        return format_string

    def get_time_period(self,filename):
        openfile = self.filename
        with open(filename, "r") as inputfile:
            all_date = []
            for line in inputfile:
                all_date.append(line.strip())
                empty_line = re.match('',line).group(0)
                if empty_line:
                    break
        return all_date
		
		
    def date_decrent(self,date):
        date_format = datetime.strptime(date, "%Y%m%d%H%M%S")
        modified_date = date_format + timedelta(days=7)
        return modified_date

    def get_events_for_day(self, dt):       
        date = self.get_start_end(dt)
        all_date = self.get_time_period(self.filename)
        valued_date = self.get_time_dictionary(all_date,date)
        if valued_date != {}:
            day_event = self.print_format(valued_date)
        else:
            day_event = ''
        return day_event
