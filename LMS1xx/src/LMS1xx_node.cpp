/*
 * LMS1xx.cpp
 *
 *  Created on: 09-08-2010
 *  Author: Konrad Banachowicz
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

#include <csignal>
#include <cstdio>
#include <LMS1xx/LMS1xx.h>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <limits>
#include <string>

#include "laser_msgs/lidar_srv.h"
#include "SysStateTypes.h"

#define DEG2RAD M_PI / 180.0
#define RAD2DEG 180.0 / M_PI

#define M2MM(X) X * 1000
#define MM2M(X) X / 1000

#define DATACOMP 32.5

// #define DATACOMP 0

lidar_status_t lidar_status  = lidar_disconn;
sys_status_t   system_status = sys_init;

bool statusCallback(laser_msgs::lidar_srv::Request & req,
                    laser_msgs::lidar_srv::Response& res);

int  main(int argc, char **argv)
{
  // laser data
  LMS1xx  laser;
  scanCfg cfg;
  scanOutputRange outputRange;
  scanDataCfg     dataCfg;
  sensor_msgs::LaserScan scan_msg;
  sensor_msgs::LaserScan scan_msg_comp;

  // parameters
  std::string host;
  std::string frame_id;
  bool inf_range;
  int  port;
  int  scan_freq;

  ros::init(argc, argv, "lms1xx");
  ros::NodeHandle nh;
  ros::NodeHandle n("~");
  ros::Publisher  scan_pub      = nh.advertise<sensor_msgs::LaserScan>("scan", 1);
  ros::Publisher  scan_comp_pub = nh.advertise<sensor_msgs::LaserScan>(
    "scan_comp",
    1);

  //
  ros::ServiceServer lidar_server =
    nh.advertiseService("lidar_status", statusCallback);

  n.param<std::string>("host",                     host,      "192.168.0.8");
  n.param<std::string>("frame_id",                 frame_id,  "laser");
  n.param<bool>(       "publish_min_range_as_inf", inf_range, false);
  n.param<int>(        "port",                     port,      2111);
  n.param<int>(        "scan_frequency",           scan_freq, 2500);

  // Set Loop Frequency
  // Do not need this actually
  //  double loop_freq = cfg.scaningFrequency / 100;
  ros::Rate loop_rate(25);

  // Compensating value for lidar detect, obtained from calibration
  double dist_comp = MM2M(DATACOMP);

  while (ros::ok())
  {
    ROS_INFO_STREAM("Connecting to laser at " << host);
    laser.connect(host, port);

    if (!laser.isConnected())
    {
      ROS_WARN("Unable to connect, retrying.");

      // ros::Duration(1).sleep();
      lidar_status = lidar_disconn;
      ros::spinOnce();
      loop_rate.sleep();
      continue;
    }

    ROS_DEBUG("Logging in to laser.");
    laser.login();
    cfg                         = laser.getScanCfg();
    outputRange                 = laser.getScanOutputRange();
    outputRange.angleResolution = 0.25 * 10000.0;

    if (cfg.scaningFrequency != scan_freq)
    {
      laser.disconnect();
      ROS_WARN("Unable to get laser output range. Retrying.");

      //      ros::Duration(1).sleep();

      // ros::Duration(1).sleep();
      lidar_status = lidar_disconn;
      ros::spinOnce();
      loop_rate.sleep();

      continue;
    }

    ROS_INFO("Connected to laser.");

    ROS_DEBUG(
      "Laser configuration: scaningFrequency %d, angleResolution %d, startAngle %d, stopAngle %d",
      cfg.scaningFrequency,
      cfg.angleResolution,
      cfg.startAngle,
      cfg.stopAngle);
    ROS_DEBUG(
      "Laser output range:angleResolution %d, startAngle %d, stopAngle %d",
      outputRange.angleResolution,
      outputRange.startAngle,
      outputRange.stopAngle);


    scan_msg.header.frame_id = frame_id;
    scan_msg.range_min       = 0.01;
    scan_msg.range_max       = 20.0;
    scan_msg.scan_time       = 100.0 / cfg.scaningFrequency;
    scan_msg.angle_increment =
      static_cast<double>(outputRange.angleResolution / 10000.0 * DEG2RAD);
    scan_msg.angle_min =
      static_cast<double>(outputRange.startAngle / 10000.0 * DEG2RAD - M_PI / 2);
    scan_msg.angle_max =
      static_cast<double>(outputRange.stopAngle / 10000.0 * DEG2RAD - M_PI / 2);

    scan_msg_comp.header.frame_id = frame_id;
    scan_msg_comp.range_min       = 0.01;
    scan_msg_comp.range_max       = 20.0;
    scan_msg_comp.scan_time       = 100.0 / cfg.scaningFrequency;
    scan_msg_comp.angle_increment =
      static_cast<double>(outputRange.angleResolution / 10000.0 * DEG2RAD);
    scan_msg_comp.angle_min =
      static_cast<double>(outputRange.startAngle / 10000.0 * DEG2RAD - M_PI / 2);
    scan_msg_comp.angle_max =
      static_cast<double>(outputRange.stopAngle / 10000.0 * DEG2RAD - M_PI / 2);

    ROS_DEBUG_STREAM(
      "Device resolution is " << (double)outputRange.angleResolution / 10000.0 <<
        " degrees.");
    ROS_DEBUG_STREAM(
      "Device frequency is " << (double)cfg.scaningFrequency / 100.0 << " Hz");

    int angle_range = outputRange.stopAngle - outputRange.startAngle;
    int num_values  = angle_range / outputRange.angleResolution;

    if (angle_range % outputRange.angleResolution == 0)
    {
      // Include endpoint
      ++num_values;
    }
    scan_msg.ranges.resize(num_values);
    scan_msg.intensities.resize(num_values);

    scan_msg.time_increment =
      (outputRange.angleResolution / 10000.0)
      / 360.0
      / (cfg.scaningFrequency / 100.0);


    scan_msg_comp.ranges.resize(num_values);
    scan_msg_comp.intensities.resize(num_values);

    scan_msg_comp.time_increment =
      (outputRange.angleResolution / 10000.0)
      / 360.0
      / (cfg.scaningFrequency / 100.0);

    ROS_DEBUG_STREAM("Time increment is " <<
                     static_cast<int>(scan_msg.time_increment * 1000000) <<
                     " microseconds");

    dataCfg.outputChannel  = 1;
    dataCfg.remission      = true;
    dataCfg.resolution     = 1;
    dataCfg.encoder        = 0;
    dataCfg.position       = false;
    dataCfg.deviceName     = false;
    dataCfg.outputInterval = 1;

    ROS_DEBUG("Setting scan data configuration.");
    laser.setScanDataCfg(dataCfg);

    ROS_DEBUG("Starting measurements.");
    laser.startMeas();

    ROS_DEBUG("Waiting for ready status.");
    ros::Time ready_status_timeout = ros::Time::now() + ros::Duration(5);

    // while(1)
    // {
    status_t stat = laser.queryStatus();
    ros::Duration(1.0).sleep();

    if (stat != ready_for_measurement)
    {
      ROS_WARN("Laser not ready. Retrying initialization.");
      laser.disconnect();

      lidar_status = lidar_disconn;

      //      ros::Duration(1).sleep();
      ros::spinOnce();
      loop_rate.sleep();
      continue;
    }

    /*if (stat == ready_for_measurement)
       {
       ROS_DEBUG("Ready status achieved.");
       break;
       }

       if (ros::Time::now() > ready_status_timeout)
       {
        ROS_WARN("Timed out waiting for ready status. Trying again.");
        laser.disconnect();
        continue;
       }

       if (!ros::ok())
       {
        laser.disconnect();
        return 1;
       }
       }*/

    ROS_DEBUG("Starting device.");
    laser.startDevice(); // Log out to properly re-enable system after config

    ROS_DEBUG("Commanding continuous measurements.");
    laser.scanContinous(1);

    while (ros::ok())
    {
      ros::Time start = ros::Time::now();

      scan_msg.header.stamp = start;
      ++scan_msg.header.seq;

      scan_msg_comp.header.stamp = start;
      ++scan_msg_comp.header.seq;

      scanData data;
      ROS_DEBUG("Reading scan data.");

      if (laser.getScanData(&data))
      {
        for (int i = 0; i < data.dist_len1; i++)
        {
          // float range_data = data.dist1[i] * 0.001;
          // Add lidar detection compensation
          float range_data_comp = data.dist1[i] * 0.001 + dist_comp;
          float range_data      = data.dist1[i] * 0.001;

          if (inf_range && (range_data < scan_msg.range_min))
          {
            scan_msg.ranges[i] = std::numeric_limits<float>::infinity();
          }
          else
          {
            scan_msg.ranges[i]      = range_data;
            scan_msg_comp.ranges[i] = range_data_comp;
          }
        }

        for (int i = 0; i < data.rssi_len1; i++)
        {
          scan_msg.intensities[i]      = data.rssi1[i];
          scan_msg_comp.intensities[i] = data.rssi1[i];
        }

        ROS_DEBUG("Publishing scan data.");
        scan_pub.publish(scan_msg);
        scan_comp_pub.publish(scan_msg_comp);
      }
      else
      {
        ROS_ERROR(
          "Laser timed out on delivering scan, attempting to reinitialize.");
        break;
      }

      lidar_status = lidar_conn;

      // Do not need this to keep loop frequency
      ros::spinOnce();
      loop_rate.sleep();
    }

    laser.scanContinous(0);
    laser.stopMeas();
    laser.disconnect();
  }

  return 0;
}

bool statusCallback(laser_msgs::lidar_srv::Request & req,
                    laser_msgs::lidar_srv::Response& res) {
  // transfer system status to node
  uint8_t system_status_now = req.sys_status;

  system_status = (sys_status_t)system_status_now;

  // transfer node status to system
  bool lidar_status_now = lidar_status;
  res.lidar_conn = lidar_status_now;

  return true;
}
