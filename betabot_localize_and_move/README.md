# Betabot Localization and Navigation

- [Betabot Localization and Navigation](#betabot-localization-and-navigation)
  - [Project Description](#project-description)
  - [GUID](#guid)
  - [Project Rubric](#project-rubric)
  - [After you implement the pkg state your reflection below](#after-you-implement-the-pkg-state-your-reflection-below)
  - [Part I: Map (map_server)](#part-i-map-mapserver)
    - [In your own words how maps are represented in ROS?](#in-your-own-words-how-maps-are-represented-in-ros)
    - [State 5 algorithm for SLAM and the ROS implementation](#state-5-algorithm-for-slam-and-the-ros-implementation)
  - [Part II: Localization (amcl)](#part-ii-localization-amcl)
    - [In your own words how amcl works?](#in-your-own-words-how-amcl-works)
    - [Is there other better approaches to localize your robot rather than amcl?](#is-there-other-better-approaches-to-localize-your-robot-rather-than-amcl)
    - [What are the amcl limitations or the fail cases?](#what-are-the-amcl-limitations-or-the-fail-cases)
    - [Is amcl used in domain rather than Robotics?](#is-amcl-used-in-domain-rather-than-robotics)
  - [i couldn't find another domain that uses amacl specifically , but it's generally used to appriximate non linear filters to estimate the state of the dinmaical systems.](#i-couldnt-find-another-domain-that-uses-amacl-specifically--but-its-generally-used-to-appriximate-non-linear-filters-to-estimate-the-state-of-the-dinmaical-systems)
  - [Part III: Navigation (move_base)](#part-iii-navigation-movebase)
    - [How many official local planner available in ROS?](#how-many-official-local-planner-available-in-ros)
    - [which local planner did you use?](#which-local-planner-did-you-use)
    - [In your own words how the local planner you selected works?](#in-your-own-words-how-the-local-planner-you-selected-works)
    - [How many official global planner available in ROS?](#how-many-official-global-planner-available-in-ros)
    - [which global planner did you use?](#which-global-planner-did-you-use)
    - [In your own words how the global planner you selected works?](#in-your-own-words-how-the-global-planner-you-selected-works)
    - [State your suggestion increase the overall performance?](#state-your-suggestion-increase-the-overall-performance)
  - [### List the most time consuming problems you faced](#h3-id%22list-the-most-time-consuming-problems-you-faced-58%22list-the-most-time-consuming-problems-you-facedh3)
    - [Demos](#demos)
    - [Screenshots](#screenshots)
      - [NAME: Khaled Osama](#name-khaled-osama)
      - [ID: 201600515](#id-201600515)

## Project Description 

Create a ROS package with custom launch files to localize the robot in an environment given the map and also to autonomous navigation. 

*Use the given [map](map/map2d.yaml) for the myoffice world [here](../betabot_gazebo/worlds/myoffice.world)* 

<p float="center">
  <img src="img/myoffice.png" width="589" /> 
  <img src="img/map2d.png" width="500" />
</p>


>NOTE: For the given map and a world the betabot robot should localize it self and move from currant pose to a given goal autonomously

## GUID
Follow where am I project from Udacity Software Robotics Engineer Nanodegree.

---
## Project Rubric

![rubric](img/reviews.jpg) 

---

## After you implement the pkg state your reflection below 

---
## Part I: Map (map_server)

###  In your own words how maps are represented in ROS?


###  State 5 algorithm for SLAM and the ROS implementation

| SLAM Algorithm | ROS implementation |
|:--------------:|:------------------:|
|     GMapping   |      gmapping      |
|                |                    |
|                |                    |
|                |                    |
|                |                    |

---

## Part II: Localization (amcl)

### In your own words how amcl works?
amcl basically implelments Monte Carlo localization approach .It takes laser scan data along with pose estimation blief and ubte the next belief accordingly until it converges to the best estimate of the robot pose.

### Is there other better approaches to localize your robot rather than amcl?
it depends on the conditions and linmitations and what you want to achieve , but a general better approach is to fuse multpile algorims together to get better estimate than all of the individual algorithms.  a good design would make use of camera,amcl and odmotry at once to get better results.

### What are the amcl limitations or the fail cases?
1. it has limited pose accuracy because of the nonconvexity of the laser sensor model, so we have a threshold of best acieved accurasy.
2. amcl is limited to local and global localization only , so if the robot shut dow for a while or got kidnapped , amcl won't be able to localize the robot's position in the map.

### Is amcl used in domain rather than Robotics?
i couldn't find another domain that uses amacl specifically , but it's generally used to appriximate non linear filters to estimate the state of the dinmaical systems.
---

## Part III: Navigation (move_base)

### How many official local planner available in ROS?

### which local planner did you use?

### In your own words how the local planner you selected works?

### How many official global planner available in ROS?

### which global planner did you use?

### In your own words how the global planner you selected works?


---

### State your suggestion increase the overall performance?
1. increase the number of particles if you want more accuracy in location , but reduce it if you want higher speed.
2. make better map
3. either make a smaller map or give the robot some time to explre the whole map.

### List the most time consuming problems you faced
---
a local problem for because of an error i have made earlier (i changed the catking workspace to root mode) ,and it made the /scan topic stop working properly
### Demos
Add unlisted youtube/drive videos

[Demo](yourlinkhere)

### Screenshots
1. rviz with all navigation launchers from turtulbot
2. gazebo

![image](src/betabot/betabot_localize_and_move/rvizsimulation.png)

---

#### NAME: Khaled Osama
#### ID: 201600515

---
