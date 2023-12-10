#!/usr/bin/env python3

import rospy
from reel_euro2021.msg import mission
import json
from std_msgs.msg import String
import time
class Mission:
    print("""Instanciationg Class Mission """)
    missions= [] 
    x=0.0
    y=0.0
    teta=0.0
    score_kbir= 0.0
    score_s8ir= 0.0
    robot= 0
    done =False
    reachable=True
    name =""
            

    @staticmethod
    def initialize():
        fileObject = open("~/AeroBotiX_euro2021/src/reel_euro2021/scripts/missions.json", "r")
        aList = json.load(fileObject)  
        tab=[]
        for element in aList["missions_complex"]:
            m=Mission(element["x"],element["y"],element["teta"],element["name"])  
            m.robot=element["robot"]
            m.done=element["done"]
            m.reachable=element["reachable"]
            m.score_kbir=element["score_kbir"]
            m.score_s8ir=element["score_s8ir"]
            m.missions=element["missions"]
            tab.append(m)
        fileObject.close()
        return tab
    def __init__(self,x,y,teta,name):
        self.x=x
        self.y=y
        self.teta=teta
        self.name=name 
        self.reachable=True
        self.missions=[]
        self.missions.append(self.name)
        self.done=False
        self.score_kbir =0.0
        self.score_s8ir =0.0
        self.robot=0
        

    def addMission(self,m):
        self.missions.append(m)
    
    def run_mission(self):
        for mission in self.missions :
            mission.execute()
        print("Mission : --> "+self.name+" <-- Executed Successfully ...")

    def execute(self):
        # the main mission here
        print("Execution Mission : --> "+self.name+" <-- ...")    
        for i in [1,2,3,4,5]:
            print(i)
            time.sleep(1)    
            
            
            
pub_kbir = rospy.Publisher('/mission_kbir', String, queue_size=10)
pub_s8ir = rospy.Publisher('/mission_s8ir', String, queue_size=10)

missions=[]



    
def updateScores():
    #get Mission
    maxKbir=0.0
    maxS8ir=0.0  
    s8ir_mission="home"
    kbir_mission="home"
         
    # update 
    for element in missions:
        if element.done==True:
            element.score_kbir=-1
            element.score_s8ir=-1
        if element.robot==1:
            element.score_s8ir=-1
        if element.robot==2:
            element.score_kbir=-1

    # PUBLISH
    for element in missions:
        if element.score_kbir > maxKbir:
            maxKbir=element.score_kbir
            kbir_mission=element.name
        if element.score_s8ir > maxS8ir:
            maxS8ir=element.score_s8ir
            s8ir_mission=element.name
    print(kbir_mission)
    print(s8ir_mission)
    pub_kbir.publish(kbir_mission)
    pub_s8ir.publish(s8ir_mission)
    

def callback(data):
    for element in missions:
        if element.name==data.name:
            element.reachable=data.reachable
            element.done=data.done
            element.robot=data.robot
    updateScores()
    
    

    
def listener():
    
    rospy.init_node('mission_node', anonymous=True)

    rospy.Subscriber("missions_topic", mission, callback)
    
    updateScores()
    # spin() simply keeps python from exiting until this node is stopped
    
    rospy.spin()

if __name__ == '__main__':
    missions=Mission.initialize()
    
    listener()