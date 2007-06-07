#!/usr/bin/env @PYTHONPROG@
"""
followup utilities

$Id$

This
"""

__author__ = 'Chad Hanna <channa@phys.lsu.edu>'
__date__ = '$Date$'
__version__ = '$Revision$'[11:-2]



import sys
import os
import copy
import re
import exceptions
import glob
import fileinput
import linecache
import string
import random
from optparse import *
from types import *
import matplotlib
matplotlib.use('Agg')
import operator
from UserDict import UserDict

from pylab import *
from glue import segments
from glue import segmentsUtils
from glue.ligolw import ligolw
from glue.ligolw import table
from glue.ligolw import lsctables
from glue.ligolw import utils
from pylal import CoincInspiralUtils
from glue import pipeline
from glue import lal


########## CLASS TO WRITE LAL CACHE FROM HIPE OUTPUT #########################
class getCache(UserDict):
  """
  An instance of a lal cache
  """
  def __init__(self, options):
    UserDict.__init__(self)
    self.dir = os.listdir(os.getcwd())
    self.options = options
    self.types = ['TMPLTBANK', 'TRIGBANK', 'INSPIRAL-', \
                 'INSPIRAL_', 'THINCA-', 'THINCA_']
    self.oNames = ['bank.cache', 'trigbank.cache', 'first_inspiral.cache', \
         'second_inspiral.cache', 'first_thinca.cache', 'second_thinca.cache']
    self.nameMaps = map(None, self.oNames, self.types)
    self.ifoTypes = ['H1','H2','L1','H1H2','H1L1','H2L1','H1H2L1']

  def ifoDict(self):
    return {'H1':[],'H2':[],'L1':[],'H1H2':[], \
                    'H1L1':[],'H2L1':[],'H1H2L1':[]}

  def getCacheType(self, type, cp=None):
    self[type] = []
    p = re.compile(type)
    f = re.compile("FOLLOWUP")
    m = re.compile("-")
    x = re.compile(".xml")
    try:
      dir = os.listdir(string.strip(cp.get('hipe-cache',type)))
      cache_path = os.path.abspath(string.strip(cp.get('hipe-cache',type)))
    except:
      dir = self.dir
      cache_path = os.path.abspath(self.options.cache_path)
    for fname in dir:
      if f.search(fname): continue
      if p.search(fname):
        ifo = m.split(fname)[0]
        start = m.split(fname)[-2]
        dur = x.split(m.split(fname)[-1])
        try:
          entry = lal.CacheEntry(ifo+" "+self.options.science_run+" " \
              +start+" "+dur[0]+" "+"file://localhost"
              +cache_path+"/"+fname)
          self[type].append(entry)
        except:
          pass

  def getCacheAll(self,cp=None):
    for type in self.types:
      self.getCacheType(type,cp)

  def writeCacheType(self,oName,type):
    cName = open(oName,'w')
    for fname in self[type]:
      cName.write(str(fname)+"\n")
    cName.close()

  def writeCacheAll(self):
    for oName, type in self.nameMaps:
      self.writeCacheType(str(oName),type)

  def filesMatchingGPS(self, time, type):
    cacheSubSet = self.ifoDict()
    for cache in self[type]:
      for ifo in self.ifoTypes:
       try:
         start = eval(str(cache).split()[2])
         end = start + eval(str(cache).split()[3])
         cacheIfo = str(cache).split()[0]
         #print cacheIfo
         if ( (end >= time[ifo]) and (start <= time[ifo]) and (cacheIfo == ifo) ):
           cacheSubSet[ifo].append(cache)
       except:
         pass
    return cacheSubSet

  def getProcessParamsFromCache(self, subCache, time):
    process = self.ifoDict()
    for ifo in subCache:
      for f in subCache[ifo]:
        doc = utils.load_filename(f.path(),None)
        proc = table.get_table(doc, lsctables.ProcessParamsTable.tableName)
        for row in proc:
          if str(row.param).find("--gps-start-time") >= 0:
             start = eval(row.value)
          if str(row.param).find("--gps-end-time") >= 0:
             end = eval(row.value)
          try:
            if ( (end >= time[ifo]) and (start <= time[ifo]) ):
              process[ifo] = proc
              break
          except: pass
    return process
    

##############################################################################
# function to read in a list of files and extract the simInspiral tables
# and sngl_inspiral tables
##############################################################################
def readFiles(fileGlob,statistic=None):
  """
  read in the Sngl and SimInspiralTables from a list of files
  if Sngls are found, construct coincs, add injections (if any)
  also return Sims (if any)
  @param fileGlob: glob of input files
  @param statistic: statistic to use in creating coincs
  """
  #if fileGlob is empty return empty structures
  if not fileGlob:
    if opts.verbose:
      print "Warning: No glob specified, returning empty structures..."
    return None, CoincInspiralUtils.coincInspiralTable()

  # if there aren't any files globbed exit
  fList = glob.glob(fileGlob)
  if not fList:
    print >>sys.stderr, "The glob for " + fileGlob + " returned no files"
    sys.exit(1)

  sims = None
  coincs = None
  for thisFile in fList:
    doc = utils.load_filename(thisFile)
    # extract the sim inspiral table
    try:
      simInspiralTable = \
          table.get_table(doc, lsctables.SimInspiralTable.tableName)
      if sims: sims.extend(simInspiralTable)
      else: sims = simInspiralTable
    except: simInspiralTable = None

    # extract the sngl inspiral table, construct coincs
    try: snglInspiralTable = \
      table.get_table(doc, lsctables.SnglInspiralTable.tableName)
    except: snglInspiralTable = None
    if snglInspiralTable:
      coincInspiralTable = \
        CoincInspiralUtils.coincInspiralTable(snglInspiralTable,statistic)
      if simInspiralTable:
        coincInspiralTable.add_sim_inspirals(simInspiralTable)
      if coincs: coincs.extend(coincInspiralTable)
      else: coincs = coincInspiralTable
  return sims,coincs



#############################################################################
# function to set up directories
#############################################################################
def setupdirs():

  try:
    os.chdir("clustered")
    os.chdir('..')
  except: os.mkdir("clustered")

  try:
    os.chdir("found")
    os.chdir('..')
  except: os.mkdir("found")

  try:
    os.chdir("missed")
    os.chdir('..')
  except: os.mkdir("missed")

  try:
    os.chdir("followuptrigs")
    os.chdir('..')
  except: os.mkdir("followuptrigs")

  try:
    os.chdir("followupfound")
    os.chdir('..')
  except: os.mkdir("followupfound")

  try:
    os.chdir("followupmissed")
    os.chdir('..')
  except: os.mkdir("followupmissed")

  try:
    os.chdir("logs")
    os.chdir('..')
  except: os.mkdir("logs")

#############################################################################
# function to return the number of slides in a file (as a string)
#############################################################################
def getslidenum(fName):
  command = "grep -m 1 -e '--num-slides' " + fName + " | sed -e 's@\"@\'@g"
  fStr = os.popen(command).readlines()
  if fStr:
    fStrlist = fStr[0].rsplit(",")
    return fStrlist[-2]
  else:
   return "0"

#############################################################################
# function to glob files
#############################################################################
def globxmlandinj(opts):

  #Glob for the files both injections and trigger files
  fList = glob.glob(opts.xml_glob)
  if not fList:
    print >>sys.stderr, "The glob for " + opts.xml_glob + " returned no files"
    sys.exit(1)

  if opts.injection_glob:
    iList = glob.glob(opts.injection_glob)
    if not iList:
      print >> sys.stderr, "The glob for " + opts.injection_glob + " returned no files"
      sys.exit(1)
  else:
    iList = None

  return fList,iList

##############################################################################
# function to coire the input files
##############################################################################
def coire(opts,fList,iList=None):
  """
  do coires
  """
  sims = None
  coincs = None

  # we need to get slide num and data type from the files
  # this is just hard coded now!!!
  for xmls in fList:
    if opts.verbose:
      print "running lalapps_coire on " + xmls
    numslides = getslidenum(xmls)
    command = "lalapps_coire " + \
              "--glob '" + xmls + "'" + \
              " --data-type " + str(opts.data_type) + \
              " --num-slides " + numslides + " --coinc-stat " + \
              opts.statistic + " --cluster-time " + \
              str(1000*opts.cluster_time)

    if iList:
      command += " --injection-file " + iList[0]
      command += " --injection-window " + str(opts.injection_coinc_time)
      command += " --missed-injections missed/MISSED" + xmls
      command += " --output found/FOUND" + xmls
    else:
      command += " --output clustered/CLUSTER"
      command += str(opts.cluster_time) + "s_" + xmls
    if opts.bitten_l_a and opts.bitten_l_b:
      command += " --h1-bittenl-a " + str(opts.bitten_l_a)
      command += " --h2-bittenl-a " + str(opts.bitten_l_a)
      command += " --l1-bittenl-a " + str(opts.bitten_l_a)
      command += " --h1-bittenl-b " + str(opts.bitten_l_b)
      command += " --h2-bittenl-b " + str(opts.bitten_l_b)
      command += " --l1-bittenl-b " + str(opts.bitten_l_b)

    os.system(command)

##############################################################################
# function to extract the statistic information
##############################################################################
def getstatistic(stat, bla, blb):

  if stat == "effective_snrsq":
    newstat = "effective_snr"
  else:
    newstat = stat

  statistic=CoincInspiralUtils.coincStatistic( newstat, bla, blb )
  return statistic


#############################################################################
# Follow up list class definition
#############################################################################
class followUpList:
  """
  Class to hold gps and ifo pairs to send to subsequent functions
  It also holds an instance of the coinc class which contains
  All of the relevant xml information
  """
  def __init__(self,Coincs = None, Missed = None ):
    self.gpsTime = {"H1" : None, "H2" : None, "L1" : None,
                  "G1" : None, "V1" : None, "T1" : None}
    self.coincs = Coincs
    self.missed = Missed
    self.eventID = None
    self.stat = None
    self.page = None
    self.summarydir = None
    self.summarypage = None
  def add_coincs(self,Coincs):
    setattr(self,"coincs",Coincs)
    self.eventID = Coincs.event_id
    self.statValue = Coincs.stat
    if self.is_trigs():
      self.summarydir = "followuptrigs"
    if self.is_found():
      self.summarydir = "followupfound"
  
  def add_missed(self,Missed):
    setattr(self,"missed",Missed)
    self.summarydir = "followupmissed"
  
  def is_trigs(self):
    if isinstance(self.coincs,CoincInspiralUtils.coincInspiralTable.row):
      return 1
  
  def is_found(self):
    sim = None
    try: 
      sim = isinstance(self.coincs.sim,lsctables.SimInspiral)
    except: return 0
    if sim:
      return 1
  def is_missed(self):
    if isinstance(self.missed,lsctables.SimInspiralTable):
      return 1
  def add_page(self,page):
    self.page = page

#############################################################################
# Function to return the follow up list of coinc triggers
#############################################################################
def getfollowuptrigs(numtrigs,page,coincs=None,missed=None):

  followups = []

  if coincs:
    sim = None
    try:
      sim = isinstance(coincs[0].sim,lsctables.SimInspiral)
    except: pass
    if sim: 
      coincs.sort(False) # This does an ascending sort instead for found inj
    else: coincs.sort()
    numTrigs = 0
    for ckey in coincs:
      fuList = followUpList()
      fuList.add_coincs(ckey)
      fuList.add_page(page)
      try:
        getattr(ckey,'H1')
        fuList.gpsTime["H1"] = (float(getattr(ckey,'H1').end_time_ns)/1000000000)+float(getattr(ckey,'H1').end_time)
      except: fuList.gpsTime["H1"] = None
      try:
        getattr(ckey,'H2')
        fuList.gpsTime["H2"] = (float(getattr(ckey,'H2').end_time_ns)/1000000000)+float(getattr(ckey,'H2').end_time)
      except: fuList.gpsTime["H2"] = None
      try:
        getattr(ckey,'L1')
        fuList.gpsTime["L1"] = (float(getattr(ckey,'L1').end_time_ns)/1000000000)+float(getattr(ckey,'L1').end_time)
      except: fuList.gpsTime["L1"] = None
      try:
        getattr(ckey,'G1')
        fuList.gpsTime["G1"] = (float(getattr(ckey,'G1').end_time_ns)/1000000000)+float(getattr(ckey,'G1').end_time)
      except: fuList.gpsTime["G1"] = None
      try:
        getattr(ckey,'V1')
        fuList.gpsTime["V1"] = (float(getattr(ckey,'V1').end_time_ns)/1000000000)+float(getattr(ckey,'V1').end_time)
      except: fuList.gpsTime["V1"] = None
      try:
        getattr(ckey,'T1')
        fuList.gpsTime["T1"] = (float(getattr(ckey,'T1').end_time_ns)/1000000000)+float(getattr(ckey,'T1').end_time)
      except: fuList.gpsTime["T1"] = None
      followups.append(fuList)
      numTrigs += 1
      if numTrigs >= numtrigs:
        break

  # the missed stuff doesnt work yet!!!
  if missed:
    followups

  return followups

#############################################################################
# Class to hold summary HTML information for all of the functions
#############################################################################
class summaryHTMLTable:

  def __init__(self,trig):
    if trig.is_trigs() and not trig.is_found():
      self.summarypath =  "followuptrigs/"
    if trig.is_trigs() and trig.is_found():
      self.summarypath = "followupfound/"
    self.eventID = trig.eventID
    self.statValue = trig.statValue
    self.H1time = trig.gpsTime["H1"]
    self.H2time = trig.gpsTime["H2"]
    self.L1time = trig.gpsTime["L1"]
    self.G1time = trig.gpsTime["G1"]
    self.V1time = trig.gpsTime["V1"]
    self.T1time = trig.gpsTime["T1"]
    self.containers = []

class HTMLcontainer:

  def __init__(self,trig,name,alt_web=None):
    # The missed injections dont work yet!!!
    self.name = name.rsplit(".")[-1]
    self.detailpath = ""
    if trig.is_trigs() and not trig.is_found():   
      os.chdir("followuptrigs")
      try: 
        os.chdir(self.name)
        os.chdir("../../")
      except: 
        os.mkdir(self.name)
        os.chdir("../")
      self.detailpath = trig.page + "/followuptrigs/" + self.name + "/"
      self.localdetailpath = "followuptrigs/" + self.name + "/"

    if trig.is_trigs() and trig.is_found():
      os.chdir("followupfound")
      try:
        os.chdir(self.name)
        os.chdir("../../")
      except:
        os.mkdir(self.name)
        os.chdir("../")
      self.detailpath = trig.page + "/followupfound/" + self.name + "/"
      self.localdetailpath = "followupfound/" + self.name + "/"

    self.image = self.detailpath + str(trig.statValue) + "_" + str(trig.eventID) + "_" + self.name + ".png"
    self.localimage = self.localdetailpath + str(trig.statValue) + "_" + str(trig.eventID) + "_" + self.name + ".png" 
    self.text = "click here"
    if alt_web: name = alt_web
    else: name = self.name
    self.link = self.detailpath + str(trig.statValue) + "_" + str(trig.eventID) + "_" + name + ".html"
    self.locallink = self.localdetailpath + str(trig.statValue) + "_" + str(trig.eventID) + "_" + name + ".html"



##############################################################################
# Function to write the HTML tables to pages
##############################################################################
def writeIULHeader(file):
  file.write('<%method title>Follow Up Report</%method><%method headline>Follow Up Report</%method><%method cvsid>$Id$</%method>\n')

def beginSummaryTable(file, table):
  file.write("<h3>Trigger [" + str(table.eventID) + 
             "] with combined statistic = " + str(table.statValue) + "</h3>")
  file.write('\n<br><table width=800 border=1>')
  
def endSummaryTable(file, table):
  file.write('\n</table>')

def writeModule(file, container):
  file.write('\n<tr><td width=400><font color="red" size=5>'+
             container.name + '</font>')
  file.write('\n<br><a href= "' + container.link + '">' + container.text 
             + "</a></td>")
  file.write('\n<td><a href= "' + container.image + '">\n<img src="' + 
              container.image + '" width=400 alt="No Image"></a></td></tr>')

def writeHTMLTables(summaryHTMLlist):
  for table in summaryHTMLlist:
    tableFile = open(table.summarypath + str(table.statValue) + "_" + 
                      str(table.eventID) + "_summary.html" ,'w')
    beginSummaryTable(tableFile,table)
    for container in table.containers:
      writeModule(tableFile, container)
    endSummaryTable(tableFile,table)
    writeIULHeader(tableFile)
    tableFile.close()

class HTMLTable:
  def __init__(self):
    self.columns = []
    self.headers = []

  def add_column(self,rows,header):
    self.columns.append(rows)
    self.headers.append(header)

  def write(self,file):
    file.write('\n<br><table><tr>')
    cnt = 0
    for i in self.columns:
      file.write('<td><b>'+ self.headers[cnt]+'</b><table>')
      cnt +=1
      for j in i:
        file.write('<tr><td>'+str(j)+'</td></tr>\n')
      file.write('</table></td>')
    file.write('\n</tr></table><br>\n')



##############################################################################
# Function to publish the web tree
##############################################################################
def publishToIULGroup(page):
  indexFile = open("index.html","w")
  patt = re.compile('.*summary.html')
  
  # First do the found injections
  files = os.chdir('followupfound')
  files = os.listdir('.')
  table = HTMLTable()
  fList = []
  stat = []
  ID = []
  if files:
    for f in files:
      temp = patt.match(f)
      if temp:
        fList.append((float(temp.group().rsplit('_')[0]), \
                  temp.group().rsplit('_')[1]))
    indexFile.write('<h3>Found Injection follow ups</h3>\n')
    sortedF = sorted(fList,key=operator.itemgetter(0),reverse=False)
    for i in sortedF:
      stat.append(str(i[0]))
      ID.append('<a href="' +page+ '/followupfound/'+str(i[0])+'_'+str(i[1])+
                '_summary.html">'+i[1]+'</a>')
    table.add_column(stat,'Stat Value')
    table.add_column(ID,'ID')
    table.write(indexFile)  
  os.chdir('..')

# then do trigs
  files = os.chdir('followuptrigs')
  files = os.listdir('.')
  table = HTMLTable()
  fList = []
  stat = []
  ID = []
  if files:
    for f in files:
      temp = patt.match(f)
      if temp:
        fList.append((float(temp.group().rsplit('_')[0]), \
                  temp.group().rsplit('_')[1]))
    indexFile.write('<h3>Found Trigger follow ups</h3>\n')
    sortedF = sorted(fList,key=operator.itemgetter(0),reverse=False)
    for i in sortedF:
      stat.append(str(i[0]))
      ID.append('<a href="' +page+ '/followuptrigs/'+str(i[0])+'_'+str(i[1])+
                '_summary.html">'+i[1]+'</a>')
    table.add_column(stat,'Stat Value')
    table.add_column(ID,'ID')
    table.write(indexFile)
  os.chdir('..')



#  # Then do the triggers 
#  files = os.chdir('followuptrigs')
#  files = os.listdir('.')
#  table = HTMLTable()
#  fList = []
#  stat = []
#  ID = []
#  if files:
#    for f in files:
#      temp = patt.match(f)
#      if temp:
#        fList.append((float(temp.group().rsplit('_')[0]), \
#                  temp.group().rsplit('_')[1]))
#    indexFile.write('<h3>Trigger follow ups</h3>\n')
#    sortedF = sorted(fList,key=operator.itemgetter(0),reverse=True)
#    for i in sortedF:
#      stat.append(str(i[0]))
#      ID.append('<a href="' +page+ '/followuptrigs/'+str(i[0])+'_'+str(i[1])+
#                '_summary.html">'+i[1]+'</a>')    
#    table.add_column(stat,'Stat Value')
#    table.add_column(ID,'ID')
#    table.write(indexFile)
#  
#  os.chdir('..')

  writeIULHeader(indexFile)
  indexFile.close()
  #This needs to be done so that it doesn't keep writing over this 
  #directory... ;)
  os.system('scp -r index.html followuptrigs followupfound followupmissed ' +
            'hydra.phys.uwm.edu:/home/htdocs/uwmlsc/root/iulgroup/'+
            'investigations/s5/people/followups/.')
  
