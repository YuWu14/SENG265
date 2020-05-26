#!/usr/bin/env python3

import sys
import argparse


# The code below configures the argparse module for use with
# assignment #2.
# 
import time
from datetime import datetime, timedelta
from datetime import datetime
import operator


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='file to be processed')
    parser.add_argument('--start', type=str, help='start of date range')
    parser.add_argument('--end', type=str, help='end of data range')

    args = parser.parse_args()

    print ('file: {}; start: {}; end: {}'.format( args.file, args.start,
        args.end))

    if not args.file:
        print("Need --file=<ics filename>")

    if not args.start:
        print("Need --start=dd/mm/yyyy")

    if not args.end:
        print("Need --end=dd/mm/yyyy")
	
	# This is the starting of my code
    filename = args.file

    with open(filename, "r") as inputfile:
        firstlist = []
        for line in inputfile:
            firstlist.append(line.strip())
            if line != None:
                continue

    start = datetime.strptime(args.start,"%d/%m/%Y")
    start = start.strftime("%Y%m%d%H%M%S")

    end = datetime.strptime(args.end,"%d/%m/%Y")
    end = end.strftime("%Y%m%d235959")
	
    Time_period(firstlist,start,end)



def dt_decrent(s):
    date = datetime.strptime(s, "%Y%m%d%H%M%S")
    modified_date = date + timedelta(days=7)
    #datetime.strftime(modified_date, "%Y%m%d%H%M%S")
    return modified_date
#'2004/03/31'

def Repeated_date(B,C,D):
    repeated = []
    #B = datetime.strptime(B,"%Y%m%d%H%M%S")
    #print(B,C,D)
    while B<D:
        b = dt_decrent(B)
        b = b.strftime("%Y%m%d%H%M%S")
		
        if b < D:
            repeated.append(b)
			
        c = dt_decrent(C)
        c = c.strftime("%Y%m%d%H%M%S")
        if c < D:
            repeated.append(c)
			
        C = c
        B = b
    #print(repeated)
    return repeated

    #b = dt_decrent(b)
  
	
def catch_date(location,summary,start,end):
    location = sorted(location.items(),key=operator.itemgetter(0))
    summary = sorted(summary.items(),key=operator.itemgetter(0))
    
    location = dict(location)
    summary = dict(summary)	
    #print(summary)
    date_period = { k: summary[k]+" "+"["+location[k]+"]" for k in location}
    #print(date_period)
	
    select_period = {}
    for k in date_period:
        if k >= start and k <= end:
            select_period.update({ k : date_period[k]})
    return select_period
	
	
def Print_format(dic):
    key_list = list(dic)
    print(key_list)
    print_list = [] 
    same_day = {}	
    i = 0
    x = key_list[i]
    y = key_list[i+1]
    for k in dic:
        if k == x:
            same_day.update({k:dic[k]})		
    while True:
        if x[0:8] == y[0:8]:
            for k in dic:
                if k == y:
                    same_day.update({k:dic[k]})					
            i = i + 1	
            if i == len(key_list):
                if x[0:8] == y[0:8]:
                    for k in dic:
                        if k == y:
                            same_day.update({k:dic[k]})
                            print_list.append(same_day)
						   
            if i >= len(key_list):
                break
            else:
                y = key_list[i]
        else:
            x = y
            i = i - 1
            print_list.append(same_day)
            same_day = {}
      	
    for dic in print_list:
        Print_day(dic)
 
 
def Print_day(dic):
    key_list = list(dic)
    day = datetime.strptime(key_list[0], "%Y%m%d%H%M%S")
    day = day.strftime('%B %d, %Y(%a)')	
    print("-"*len(day))
    i = 0

    while True:
        x = key_list[i]
        y = key_list[i+1]
        DS = datetime.strptime(x, "%Y%m%d%H%M%S") 
        DT = datetime.strptime(y, "%Y%m%d%H%M%S")  		
        DS = DS.strftime('%-H:%M %p')
        DT = DT.strftime('%-H:%M %p')
        for k in dic:
            if k == x:
                m = dic[k]
        if "same_time_case" in m:
            print(DS + " to " + DT + ":" + m[0:8] + m[40:48]+"]")           		
            print(DS + " to " + DT + ":" + m[8:25] +"["+ m[48:56]+"]") 		
        else:            		
            print(DS + " to " + DT + ":" + m)
        i = i + 2
        if i == len(key_list):
            break

        		
		

def merge_dict(list_dictionary):
    onedic = {}
    for d in list_dictionary:
        for k in d:
            if k not in onedic:
                onedic.update({k:d[k]})
            else:
                onedic[k] = onedic[k]+d[k]+"same_time_case"
    return onedic


def Time_period(A,start,end):
    secondlist = []
    c = 0
    location = {}
    summary = {}
    flist = []
    flist1 = []
    for x in A:
        if "DTSTART" in x:
            secondlist.append(x[8:16]+x[17:])			
        if "DTEND" in x:
            secondlist.append(x[6:14]+x[15:])			
        if "UNTIL" in x:
            c = len(secondlist)
            dt = secondlist[c-2]
            de = secondlist[c-1]
            repeatedlist = Repeated_date(dt,de,x[32:40]+x[41:47])
            for x in repeatedlist:
                secondlist.append(x)				
        if "LOCATION" in x:
            location.update ({ i : x[9:] for i in secondlist})
            flist.append(location)
            location = {}			
        if "SUMMARY" in x:
            summary.update ({ i : x[8:] for i in secondlist})	
            flist1.append(summary)
            summary = {}
            secondlist = []
    #print(flist1)
    location = merge_dict(flist)
    summary = merge_dict(flist1)
    #print(summary)
    final_dic = catch_date(location,summary,start,end)
    Print_format(final_dic)





if __name__ == "__main__":
    main()
