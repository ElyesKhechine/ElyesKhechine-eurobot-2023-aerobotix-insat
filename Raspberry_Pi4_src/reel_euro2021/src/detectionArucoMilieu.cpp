#include <ros/ros.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <std_msgs/Int32.h>

// Global variables
ros::Publisher pub;
std_msgs::Int32 presence;
std::string codeCouleurArucoMilieu = "42";
bool calibrated = false;
int i = 0;

// Translation
float x[10] = {};
float y[10] = {};
float z[10] = {};
// Rotation
float qx[10] = {};
float qy[10] = {};
float qz[10] = {};
float qw[10] = {};

// coordonnees aruco ordo jaune
// reference: map
/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
    indice 2 --> y min
    indice 3 --> y max
*/
float coordonneesArucoJauneOrdo[] = {0.595, 1.195, 0.375, 0.975};

// coordonnees aruco non ordo jaune
// reference: map
/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
    indice 2 --> y min
    indice 3 --> y max
*/
float coordonneesArucoJauneNonOrdo[] = {1.2, 1.550, 0.8, 1.115};

// coordonnees aruco ordo violet
// reference: map
/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
    indice 2 --> y min
    indice 3 --> y max
*/
float coordonneesArucoVioletOrdo[] = {1.805, 2.405, 0.375, 0.975};

// coordonnees aruco non ordo violet
// reference: map
/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
    indice 2 --> y min
    indice 3 --> y max
*/
float coordonneesArucoVioletNonOrdo[] = {1.2, 1.55, 1.85, 2.2};

int checkJauneOrdo(float x, float y)
{
    if ((x >= coordonneesArucoJauneOrdo[0]) && x <= (coordonneesArucoJauneOrdo[1]) && y >= coordonneesArucoJauneOrdo[2] && y <= coordonneesArucoJauneOrdo[3])
        return 1;
    else
        return 0;
}

int checkJauneNonOrdo(float x, float y)
{
    if ((x >= coordonneesArucoJauneNonOrdo[0]) && x <= (coordonneesArucoJauneNonOrdo[1]) && y >= coordonneesArucoJauneNonOrdo[2] && y <= coordonneesArucoJauneNonOrdo[3])
        return 10;
    else
        return 0;
}

int checkVioletOrdo(float x, float y)
{
    if ((x >= coordonneesArucoVioletOrdo[0]) && x <= (coordonneesArucoVioletOrdo[1]) && y >= coordonneesArucoVioletOrdo[2] && y <= coordonneesArucoVioletOrdo[3])
        return 100;
    else
        return 0;
}

int checkVioletNonOrdo(float x, float y)
{
    if ((x >= coordonneesArucoVioletNonOrdo[0]) && x <= (coordonneesArucoVioletNonOrdo[1]) && y >= coordonneesArucoVioletNonOrdo[2] && y <= coordonneesArucoVioletNonOrdo[3])
        return 1000;
    else
        return 0;
}

float moyenne(float val[])
{
    float somme = 0;
    for (int i = 0; i < 10; i++)
    {
        somme += val[i];
    }
    return (somme / 10);
}

void broadcaster(geometry_msgs::TransformStamped &coordonneesAruco)
{
    static tf2_ros::StaticTransformBroadcaster tf_broadcaster;
    coordonneesAruco.header.stamp = ros::Time::now();
    coordonneesAruco.header.frame_id = "detectionArucoMileuCameraIp";
    coordonneesAruco.child_frame_id = "arucoMilieu";
    coordonneesAruco.transform.translation.x = moyenne(x);
    coordonneesAruco.transform.translation.y = moyenne(y);
    coordonneesAruco.transform.translation.z = moyenne(z);
    coordonneesAruco.transform.rotation.x = moyenne(qx);
    coordonneesAruco.transform.rotation.y = moyenne(qy);
    coordonneesAruco.transform.rotation.z = moyenne(qz);
    coordonneesAruco.transform.rotation.w = moyenne(qw);

    // ROS_INFO("Final: %s %f %f %f %f %f %f %f", coordonneesAruco.child_frame_id.c_str(), coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y, coordonneesAruco.transform.translation.z, coordonneesAruco.transform.rotation.x, coordonneesAruco.transform.rotation.y, coordonneesAruco.transform.rotation.z, coordonneesAruco.transform.rotation.w);

    tf_broadcaster.sendTransform(coordonneesAruco);
}

void detectAruco(const tf2_msgs::TFMessage::ConstPtr &trans)
{
    static tf2_ros::Buffer tfBuffer;
    static tf2_ros::TransformListener tfListener(tfBuffer);
    static geometry_msgs::TransformStamped coordonneesAruco;

    int nbArucoJaunceOrdo{0};
    int nbArucoJauneNonOrdo{0};
    int nbArucoVioletOrdo{0};
    int nbArucoVioletNonOrdo{0};

    presence.data = 0;

    if (calibrated == false)
    {
        ROS_INFO("Calibrating camera milieu");
        if (i < 10)
        {
            for (auto j = 0; j < trans->transforms.size(); ++j)
            {
                if (trans->transforms[j].child_frame_id.find("17") != std::string::npos)
                {
                    try
                    {
                        coordonneesAruco = tfBuffer.lookupTransform("detectionArucoMileuCameraIp", trans->transforms[j].child_frame_id, ros::Time(0), ros::Duration(0.5));
                    }
                    catch (tf2::TransformException ex)
                    {
                        ROS_ERROR("%s", ex.what());
                        ros::Duration(1.0).sleep();
                        continue;
                    }
                    // ROS_INFO("test: %s %f %f %f %f %f %f %f", coordonneesAruco.child_frame_id.c_str(), coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y, coordonneesAruco.transform.translation.z, coordonneesAruco.transform.rotation.x, coordonneesAruco.transform.rotation.y, coordonneesAruco.transform.rotation.z, coordonneesAruco.transform.rotation.w);
                    x[i] = coordonneesAruco.transform.translation.x;
                    y[i] = coordonneesAruco.transform.translation.y;
                    z[i] = coordonneesAruco.transform.translation.z;
                    qx[i] = coordonneesAruco.transform.rotation.x;
                    qy[i] = coordonneesAruco.transform.rotation.y;
                    qz[i] = coordonneesAruco.transform.rotation.z;
                    qw[i] = coordonneesAruco.transform.rotation.w;
                    i++;
                }
            }
        }
        else
        {
            // ROS_INFO("ArucoMilieu: %s %f %f %f", coordonneesAruco.child_frame_id.c_str(), moyenne(x), moyenne(y), moyenne(z));
            ROS_INFO("Camera milieu calibrated");
            calibrated = true;
        }
    }
    else
    {
        broadcaster(coordonneesAruco);
        for (int i = 0; i < trans->transforms.size(); ++i)
        {
            try
            {
                coordonneesAruco = tfBuffer.lookupTransform("map", trans->transforms[i].child_frame_id, ros::Time(0), ros::Duration(2.0));
            }
            catch (tf2::TransformException ex)
            {
                ROS_ERROR("Camera milieu: %s", ex.what());
                ros::Duration(1.0).sleep();
                continue;
            }
            presence.data += checkJauneOrdo(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
            presence.data += checkJauneNonOrdo(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
            presence.data += checkVioletOrdo(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
            presence.data += checkVioletNonOrdo(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
        }
        //ROS_INFO("Presence: %d", presence.data);
    }
    pub.publish(presence);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "detectionArucoMilieu");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("detectionArucoMileuCameraIp/image_raw/tf_list", 1000, detectAruco);

    pub = n.advertise<std_msgs::Int32>("presenceAruco", 10);

    ros::Rate r(1.0);
    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }

    // ros::spin();

    return 0;
}
