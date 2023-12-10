#include "ros/ros.h"
#include "reel_euro2021/map.h"
#include "std_msgs/String.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/GetMap.h"

#include <sstream>
float resol = 0.01;

nav_msgs::OccupancyGrid newMap;
nav_msgs::OccupancyGrid initialMap;
std::string color="yellow";

void putPoint(int x, int y)
{
  newMap.data[(y + 15) * 330 + x + 15] = 100;
}
void deletePoint(int x, int y)
{
  newMap.data[(y + 15) * 330 + x + 15] = 0;
}

void deleteHexagone(int xc, int yc, float r)
{

  // int xc=500;
  // int yc=100;
  // float r=7.5;

  for (int i = 0; i < 330 * 230; i++)
  {
    int y = i / 330;
    int x = i % 330;

    if (sqrt((xc - x) * (xc - x) + (yc - y) * (yc - y)) < r)
    {

      deletePoint(x, y);
    }
  }
}

void putHexagone(int xc, int yc, float r)
{

  // int xc=500;
  // int yc=100;
  // float r=7.5;

  for (int i = 0; i < 330 * 230; i++)
  {
    int y = i / 330;
    int x = i % 330;

    if (sqrt((xc - x) * (xc - x) + (yc - y) * (yc - y)) < r)
    {

      putPoint(x, y);
    }
  }
}

void putReservoir1()
{
  int xs[] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190};
  for (int i = 0; i < 36; i++)
  {
    if(color=="yellow"){
      putPoint(xs[i], ys[i]);
    }else{
      putPoint(xs[i], 200-ys[i]);
    }
  }
}

void putReservoir2()
{
  int xs[] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190};
  for (int i = 0; i < 36; i++)
  {
    if(color=="yellow"){
    putPoint(xs[i] + 30, ys[i]);}
    else{

       putPoint(xs[i] + 30, 200-ys[i]);
    }
  }
}

void putReservoir3()
{
  int ys[] = {67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82};
  int xs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
  for (int i = 0; i < 37; i++)
  {
    if(color=="yellow"){
    putPoint(xs[i], ys[i]);}
    else{
    putPoint(xs[i], 200-ys[i]);

    }
  }
}

void putGalerie1()
{
  int xs[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190};
  for (int i = 0; i < 22; i++)
  {
    if(color=="yellow"){
    putPoint(xs[i], ys[i]);}
    else{
    putPoint(xs[i], 200-ys[i]);

    }
   
  }

  for (int i = 45; i < 117; i++)
  {
    if(color=="yellow"){
    putPoint(i, 190);}
    else{
    putPoint(i,10);

    }
  }
}

void putGalerie2()
{
  int xs[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190};
  for (int i = 0; i < 22; i++)
  {
    if(color=="yellow"){
    putPoint(xs[i]+138, ys[i]);}
    else{
    putPoint(xs[i]+138, 200-ys[i]);

    }
  }

  for (int i = 45; i < 117; i++)
  {
    if(color=="yellow"){
    putPoint(i+138, 190);}
    else{
    putPoint(i+138, 10);

    }
  }
}

void putTaquet()
{
  for (int i = 200; i > 170; i--)
  {
    if(color=="yellow"){
    putPoint(149, i);
    putPoint(150, i);
    }
    else{
     putPoint(149,200- i);
    putPoint(150,200- i);

    }
  }
}

void putAbri()
{
  for (int i = 0; i <= 51; i++)
  {

if(color=="yellow"){
    putPoint(i,-i+51);}
    else{
    putPoint(i,200-(-i+51));

    }
  }
}

void deleteReservoir1()
{
  int xs[] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190};
  for (int i = 0; i < 36; i++)
  {
    if(color=="yellow"){
    deletePoint(xs[i], ys[i]);}
    else{
    deletePoint(xs[i], 200-ys[i]);

    }
  }
}

void deleteReservoir2()
{
  int xs[] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190};
  for (int i = 0; i < 36; i++)
  {
    
    if(color=="yellow"){
    deletePoint(xs[i]+30, ys[i]);}
    else{
    deletePoint(xs[i]+30, 200-ys[i]);

    }
  }
}

void deleteReservoir3()
{
  int ys[] = {67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82};
  int xs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
  for (int i = 0; i < 37; i++)
  {
    if(color=="yellow"){
    deletePoint(xs[i], ys[i]);}
    else{
    deletePoint(xs[i], 200-ys[i]);

    }

  }
}

void deleteGalerie1()
{
  int xs[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190};
  for (int i = 0; i < 22; i++)
  {
    if(color=="yellow"){
    deletePoint(xs[i], ys[i]);}
    else{
    deletePoint(xs[i], 200-ys[i]);

    }
  }

  for (int i = 45; i < 117; i++)
  {
    if(color=="yellow"){
    deletePoint(i, 190);}
    else{
    deletePoint(i, 10);

    }
  }
}

void deleteGalerie2()
{
  int xs[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117};
  int ys[] = {200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190};
  for (int i = 0; i < 22; i++)
  {
    if(color=="yellow"){
    deletePoint(xs[i]+138, ys[i]);}
    else{
    deletePoint(xs[i]+138, 200-ys[i]);

    }
  }

  for (int i = 45; i < 117; i++)
  {
    if(color=="yellow"){
    deletePoint(i+138, 190);}
    else{
    deletePoint(i+138, 10);

    }
  }
}

void deleteTaquet()
{
  for (int i = 200; i > 170; i--)
  {
    if(color=="yellow"){
    deletePoint(149, i);
    deletePoint(150, i);
    }
    else{
    deletePoint(149,200- i);
    deletePoint(150,200- i);

    }
  }
}

void deleteAbri()
{
  for (int i = 0; i <= 51; i++)
  {
if(color=="yellow"){
        deletePoint(i, -i + 51);

    }
    else{
       deletePoint(i, 200 -(-i + 51));


    }
  }
}

void initializeMap()
{
  putAbri();
  putTaquet();
  putReservoir1();
  putReservoir2();
  putReservoir3();
  putGalerie1();
  putGalerie2();
  if(color=="yellow"){
putHexagone(90,145,7.5);
  putHexagone(90,121,7.5);
  putHexagone(83,133,7.5);
  putHexagone(80,71,7.5);
  putHexagone(100,62,7.5);
  putHexagone(85,55,7.5);
  }else{
    putHexagone(90,55,7.5);
  putHexagone(90,79,7.5);
  putHexagone(83,67,7.5);
  putHexagone(80,129,7.5);
  putHexagone(100,138,7.5);
  putHexagone(85,145,7.5);
  }

  
}
void resetMap()
{
  newMap=initialMap;
}
void deleteAll()
{
  newMap=initialMap;
  for (int i = 0; i < 330*230; i++)
  {
    newMap.data[i]=0;
  }
  
}

void updateData()
{

  // putHexagone(150,150,7.5);
}

bool update(reel_euro2021::map::Request &req, reel_euro2021::map::Response &res)
{
  if (req.r == 0)
  {
    if (req.action == true)
    {
      initializeMap();
      res.result = "Done initializing Map";
    }
    else
    {
       resetMap();
      res.result = "Done Resetting Map";
    }
  }
  else if (req.r == -1)
  {
    if (req.action == true)
    {
      putReservoir1();
      res.result = "Done putting Reservoir1";
    }
    else
    {
      deleteReservoir1();

      res.result = "Done Deleting Reservoir1";
    }
  }
  else if (req.r == -2)
  {
    if (req.action == true)
    {
      putReservoir2();
      res.result = "Done putting Reservoir2";
    }
    else
    {
      deleteReservoir2();
      res.result = "Done Deleting Reservoir2";
    }
  }
  else if (req.r == -3)
  {
    if (req.action == true)
    {
      putReservoir3();
      res.result = "Done putting Reservoir3";
    }
    else
    {
      deleteReservoir3();
      res.result = "Done Deleting Reservoir3";
    }
  }
  else if (req.r == -4)
  {
    if (req.action == true)
    {
      putTaquet();
      res.result = "Done putting Taquet";
    }
    else
    {
      deleteTaquet();
      res.result = "Done Deleting Taquet";
    }
  }
  else if (req.r == -5)
  {
    if (req.action == true)
    {
      putAbri();
      res.result = "Done putting Abri";
    }
    else
    {
      deleteAbri();
      res.result = "Done Deleting Abri";
    }
  }
  else if (req.r == -6)
  {
    if (req.action == true)
    {
      putGalerie1();
      res.result = "Done putting Galerie1";
    }
    else
    {
      deleteGalerie1();
      res.result = "Done Deleting Galerie1";
    }
  }
  else if (req.r == -7)
  {
    if (req.action == true)
    {
      putGalerie2();
      res.result = "Done putting Galerie2";
    }
    
    else
    {
      deleteGalerie2();

      res.result = "Done Deleting Galerie2";
    }
  }
  else if (req.r == 99)
  {
    deleteAll();
  }
  else
  {
    if (req.action == true)
    {
      putHexagone(req.x, req.y, req.r);
      res.result = "Done putting Hexagone";
    }
    else
    {
      deleteHexagone(req.x, req.y, req.r);
      res.result = "Done deleting Hexagone";
    }
  }
  return true;
}
int main(int argc, char **argv)
{

  ros::init(argc, argv, "mapUpdate");

  ros::NodeHandle n;
   if (ros::param::has("color")){
      ros::param::get("color", color);}
      else {
          color="yellow";
      }
  ros::ServiceServer service = n.advertiseService("/updateMapSrv", update);

  ros::Publisher chatter_pub = n.advertise<nav_msgs::OccupancyGrid>("/map", 100);

  ros::ServiceClient client = n.serviceClient<nav_msgs::GetMap>("/static_map");
  nav_msgs::GetMap srv;
  while (!client.call(srv))
  {
    continue;
  }
  newMap = srv.response.map;
  initialMap=newMap;
  //   for (int i = 0; i < 230*5; i++)
  //   {
  //     newMap.data[i]=100;
  //   }

  ros::Rate loop_rate(1);

  int count = 0;
  while (ros::ok())
  {

    updateData();
    chatter_pub.publish(newMap);

    //     nav_msgs::OccupancyGrid occ;
    //     occ.info.height=1;
    //     occ.info.width=5;
    //     occ.info.resolution=resol;
    //     occ.header.stamp=ros::Time::now();
    //     occ.header.frame_id="map";
    //     occ.data={100,100,100,100,100};
    //     chatter_pub.publish(occ);
    //    // loop_rate.sleep();
    //     count++;
    //     // break;
    //         ROS_INFO("%d", count);
    ros::spinOnce();
  }

  return 0;
}