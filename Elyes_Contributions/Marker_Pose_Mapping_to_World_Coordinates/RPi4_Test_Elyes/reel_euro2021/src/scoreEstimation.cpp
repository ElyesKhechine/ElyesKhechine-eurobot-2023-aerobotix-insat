#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/static_transform_broadcaster.h>

#define minMin 0
#define maxMax 51

// Global variables
ros::Publisher pub;
std::string sommetAruco[] = {"/C1", "/C2", "/C3", "/C4", "/C5", "/C6"};
std_msgs::Int32 score;
std::string codeCouleurArucoMilieu = "etalon";
bool calibrated = false;
int i = 0;
int max = 0;
int sens = 1;
double tolerance = 1.00;
double tolerance2 = 0.95;

const int nbEchan = 20;

int currentMax = 0;

// Translation
double x[nbEchan] = {};
double y[nbEchan] = {};
double z[nbEchan] = {};
// Rotation
double qx[nbEchan] = {};
double qy[nbEchan] = {};
double qz[nbEchan] = {};
double qw[nbEchan] = {};

/*
    Code zone (galerie ou/et campement)
    blue = 1
    vert = 10
    rouge = 100
*/
int codeZone[] = {1, 10, 100};

/*
    Code aruco des couleurs
    blue = 13
    green = 36
    red = 47
    colorless = 17
*/
std::string codeCouleurAruco[] = {"13", "36", "47", "17"};

// coordonnees galerie
// Base frame: cameraEstimationScore

/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
    indice 2 --> y min
    indice 3 --> y max
*/

// double coordonneesGalerie[] = {-0.360, 0.360, -0.135, 0.135};
// double coordonneesZoneBleuGalerie[] = {-0.360, -0.120, -0.135, 0.135};
// double coordonneesZoneVertGalerie[] = {-0.120, 0.120, -0.135, 0.135};
// double coordonneesZoneRougeGalerie[] = {0.120, 0.360, -0.135, 0.135};

double coordonneesGalerie[] = {-0.360 * tolerance, 0.360 * tolerance, -0.145 * tolerance, 0.145 * tolerance};
double coordonneesZoneBleuGalerie[] = {-0.360 * tolerance, -0.120 * tolerance, -0.150 * tolerance, 0.150 * tolerance};
double coordonneesZoneVertGalerie[] = {-0.120 * tolerance, 0.120 * tolerance, -0.150 * tolerance, 0.150 * tolerance};
double coordonneesZoneRougeGalerie[] = {0.120 * tolerance, 0.360 * tolerance, -0.150 * tolerance, 0.150 * tolerance};

// coordonnees campement
// Base frame: cameraEstimationScore

/*
    Verification par rapport a l'axe des X
    indice 0 --> x min
    indice 1 --> x max
*/
double coordonneesCampement[] = {-0.28, 0.40};
double coordonneesZoneBleuCampement[] = {-0.28, 0.019};
double coordonneesZoneVertCampement[] = {-0.09, 0.2};
double coordonneesZoneRougeCampement[] = {0.09, 0.4};

/*

*/
double moyenne(double val[])
{
    double somme = 0;
    for (int i = 0; i < nbEchan; i++)
    {
        somme += val[i];
    }
    return (somme / nbEchan);
}

void broadcasterCameraGalerie(geometry_msgs::TransformStamped &coordonneesAruco)
{
    static tf2_ros::StaticTransformBroadcaster tf_broadcaster;
    coordonneesAruco.header.stamp = ros::Time::now();
    coordonneesAruco.header.frame_id = "scoreEstimationCameraIp";
    coordonneesAruco.child_frame_id = "galerie";
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
/*
    tf broadcaster
    Envoie les tf des sommets d'un element de jeux
*/
void broadcasterSixSommets(const std::string &frame_id)
{
    static tf2_ros::StaticTransformBroadcaster br;
    static geometry_msgs::TransformStamped transformStamped;

    for (unsigned int i = 0; i < 6; ++i)
    {
        if (i == 0)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = 0.0;
            transformStamped.transform.translation.y = 0.075;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
        if (i == 1)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = -0.06496;
            transformStamped.transform.translation.y = 0.0375;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
        if (i == 2)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = -0.06495;
            transformStamped.transform.translation.y = -0.0375;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
        if (i == 3)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = 0.0;
            transformStamped.transform.translation.y = -0.075;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
        if (i == 4)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = 0.06495;
            transformStamped.transform.translation.y = -0.0375;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
        if (i == 5)
        {
            transformStamped.header.stamp = ros::Time::now();
            transformStamped.header.frame_id = frame_id;
            transformStamped.child_frame_id = frame_id + sommetAruco[i];
            transformStamped.transform.translation.x = 0.06495;
            transformStamped.transform.translation.y = 0.0375;
            transformStamped.transform.translation.z = 0.0;
            transformStamped.transform.rotation.x = 0.0;
            transformStamped.transform.rotation.y = 0.0;
            transformStamped.transform.rotation.z = 0.0;
            transformStamped.transform.rotation.w = 1.0;
            br.sendTransform(transformStamped);
        }
    }
}

// void broadcasterSixSommets(const std::string &frame_id)
// {
//     static tf2_ros::TransformBroadcaster br;
//     static geometry_msgs::TransformStamped transformStamped;

//     for (unsigned int i = 0; i < 6; ++i)
//     {
//         if (i == 0)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = 0.0;
//             transformStamped.transform.translation.y = 0.075;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//         if (i == 1)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = -0.06496;
//             transformStamped.transform.translation.y = 0.0375;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//         if (i == 2)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = -0.06495;
//             transformStamped.transform.translation.y = -0.0375;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//         if (i == 3)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = 0.0;
//             transformStamped.transform.translation.y = -0.075;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//         if (i == 4)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = 0.06495;
//             transformStamped.transform.translation.y = -0.0375;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//         if (i == 5)
//         {
//             transformStamped.header.stamp = ros::Time::now();
//             transformStamped.header.frame_id = frame_id;
//             transformStamped.child_frame_id = frame_id + sommetAruco[i];
//             transformStamped.transform.translation.x = 0.06495;
//             transformStamped.transform.translation.y = 0.0375;
//             transformStamped.transform.translation.z = 0.0;
//             tf2::Quaternion q;
//             q.setRPY(0, 0, 0);
//             transformStamped.transform.rotation.x = q.x();
//             transformStamped.transform.rotation.y = q.y();
//             transformStamped.transform.rotation.z = q.z();
//             transformStamped.transform.rotation.w = q.w();
//             br.sendTransform(transformStamped);
//         }
//     }
// }

/*
    Galerie
*/

int sommetsZoneBleuGalerie(double x, double y)
{
    if ((x >= coordonneesZoneBleuGalerie[0]) && (x <= coordonneesZoneBleuGalerie[1]) && (y >= coordonneesZoneBleuGalerie[2]) && (y <= coordonneesZoneBleuGalerie[3]))
        return 1000;
    else if ((x >= coordonneesGalerie[0]) && (x <= coordonneesGalerie[1]) && (y >= coordonneesGalerie[2]) && (y <= coordonneesGalerie[3]))
        return 100;
    else
        return 0;
}

int sommetsZoneVertGalerie(double x, double y)
{
    if (x >= coordonneesZoneVertGalerie[0] && x <= coordonneesZoneVertGalerie[1] && y >= coordonneesZoneVertGalerie[2] && y <= coordonneesZoneVertGalerie[3])
        return 1000;
    else if (x >= coordonneesGalerie[0] && x <= coordonneesGalerie[1] && y >= coordonneesGalerie[2] && y <= coordonneesGalerie[3])
        return 100;
    else
        return 0;
}

int sommetsZoneRougeGalerie(double x, double y)
{
    if (x >= coordonneesZoneRougeGalerie[0] && x <= coordonneesZoneRougeGalerie[1] && y >= coordonneesZoneRougeGalerie[2] && y <= coordonneesZoneRougeGalerie[3])
        return 1000;
    else if (x >= coordonneesGalerie[0] && x <= coordonneesGalerie[1] && y >= coordonneesGalerie[2] && y <= coordonneesGalerie[3])
        return 100;
    else
        return 0;
}

int sommetsGalerie(double x, double y)
{
    if (x >= coordonneesGalerie[0] && x <= coordonneesGalerie[1] && y >= coordonneesGalerie[2] && y <= coordonneesGalerie[3])
        return 100;
    else
        return 0;
}

/*
    Campement
*/

int sommetsZoneBleuCampement(double x, double y)
{
    if ((x >= coordonneesZoneBleuCampement[0]) && (x <= coordonneesZoneBleuCampement[1]) && (y >= coordonneesZoneBleuCampement[2]) && (y <= coordonneesZoneBleuCampement[3]))
        return 10;
    else if ((x >= coordonneesCampement[0]) && (x <= coordonneesCampement[1]) && (y >= coordonneesCampement[2]) && (y <= coordonneesCampement[3]))
        return 1;
    else
        return 0;
}

int sommetsZoneVertCampement(double x, double y)
{
    if (x >= coordonneesZoneVertCampement[0] && x <= coordonneesZoneVertCampement[1] && y >= coordonneesZoneVertCampement[2] && y <= coordonneesZoneVertCampement[3])
        return 10;
    else if (x >= coordonneesCampement[0] && x <= coordonneesCampement[1] && y >= coordonneesCampement[2] && y <= coordonneesCampement[3])
        return 1;
    else
        return 0;
}

int sommetsZoneRougeCampement(double x, double y)
{
    if (x >= coordonneesZoneRougeCampement[0] && x <= coordonneesZoneRougeCampement[1] && y >= coordonneesZoneRougeCampement[2] && y <= coordonneesZoneRougeCampement[3])
        return 10;
    else if (x >= coordonneesCampement[0] && x <= coordonneesCampement[1] && y >= coordonneesCampement[2] && y <= coordonneesCampement[3])
        return 1;
    else
        return 0;
}

int sommetsCampement(double x, double y)
{
    if (x >= coordonneesCampement[0] && x <= coordonneesCampement[1] && y >= coordonneesCampement[2] && y <= coordonneesCampement[3])
        return 1;
    else
        return 0;
}

void estimationScore(const tf2_msgs::TFMessage::ConstPtr &trans)
{
    static tf2_ros::Buffer tfBuffer;
    static tf2_ros::TransformListener tfListener(tfBuffer);
    static geometry_msgs::TransformStamped coordonneesAruco;

    int nbArucoJaunceOrdo{0};
    int nbArucoJauneNonOrdo{0};
    int nbArucoVioletOrdo{0};
    int nbArucoVioletNonOrdo{0};

    int zone = 0;
    score.data = 0;

    if (calibrated == false)
    {
        ROS_INFO("Calibrating camera galerie");
        if (i < nbEchan)
        {
            for (auto j = 0; j < trans->transforms.size(); ++j)
            {
                if (trans->transforms[j].child_frame_id.find(codeCouleurArucoMilieu) != std::string::npos)
                {
                    try
                    {
                        coordonneesAruco = tfBuffer.lookupTransform("scoreEstimationCameraIp", trans->transforms[j].child_frame_id, ros::Time(0), ros::Duration(0.5));
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
            ROS_INFO("Camera galerie calibrated");
            calibrated = true;
        }
    }
    else
    {
        broadcasterCameraGalerie(coordonneesAruco);
        for (int i = 0; i < trans->transforms.size(); ++i)
        {
            broadcasterSixSommets(trans->transforms[i].child_frame_id);
            for (const std::string &sommet : sommetAruco)
            {
                try
                {
                    coordonneesAruco = tfBuffer.lookupTransform("galerie", trans->transforms[i].child_frame_id + sommet, ros::Time(0), ros::Duration(0.5));
                }
                catch (tf2::TransformException ex)
                {
                    ROS_ERROR("%s", ex.what());
                    ros::Duration(1.0).sleep();
                    continue;
                }
                //ROS_INFO("test: %s %f %f %f %f %f %f %f", coordonneesAruco.child_frame_id.c_str(), coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y, coordonneesAruco.transform.translation.z, coordonneesAruco.transform.rotation.x, coordonneesAruco.transform.rotation.y, coordonneesAruco.transform.rotation.z, coordonneesAruco.transform.rotation.w);                
                ROS_INFO("%s %f %f %f", coordonneesAruco.child_frame_id.c_str(), coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y, coordonneesAruco.transform.translation.z);

                if (trans->transforms[i].child_frame_id.find(codeCouleurAruco[0]) != std::string::npos)
                {
                    zone += sommetsZoneBleuGalerie(sens * coordonneesAruco.transform.translation.x * tolerance, coordonneesAruco.transform.translation.y * tolerance);
                    // zone += sommetsZoneBleuCampement(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
                }
                if (trans->transforms[i].child_frame_id.find(codeCouleurAruco[1]) != std::string::npos)
                {
                    zone += sommetsZoneVertGalerie(sens * coordonneesAruco.transform.translation.x * tolerance, coordonneesAruco.transform.translation.y * tolerance);
                    // zone += sommetsZoneVertCampement(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
                }
                if (trans->transforms[i].child_frame_id.find(codeCouleurAruco[2]) != std::string::npos)
                {
                    zone += sommetsZoneRougeGalerie(sens * coordonneesAruco.transform.translation.x * tolerance, coordonneesAruco.transform.translation.y * tolerance);
                    // zone += sommetsZoneRougeCampement(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
                }
                if (trans->transforms[i].child_frame_id.find(codeCouleurAruco[3]) != std::string::npos)
                {
                    zone += sommetsGalerie(sens * coordonneesAruco.transform.translation.x * tolerance, coordonneesAruco.transform.translation.y * tolerance);
                    // zone += sommetsCampement(coordonneesAruco.transform.translation.x, coordonneesAruco.transform.translation.y);
                }
            }
            if ((zone / 1000) % 10 >= 3)
                score.data += 6;
            else if ((zone / 100) >= 1)
                score.data += 3;
            // else if ((zone / 10) % 10 >= 4)
            //     score.data += 2;
            // else if ((zone % 10) >= 1)
            //     score.data += 1;
            ROS_INFO("zone: %d", zone);
            zone = 0;
        }
	ROS_INFO("SCORE = %d", score.data);
        if (score.data > currentMax)
            currentMax = score.data;

        if (currentMax > maxMax)
            currentMax = maxMax;
	score.data = currentMax;
        pub.publish(score);
    }
}

/*void setSens(const std_msgs::Int8 &couleur)
{
    if (couleur.data == 2)
    {
        sens = -1;
    }
}*/

void reset(const std_msgs::Int8 &res)
{
    if (res.data == 0)
    {
        max = 0;
        score.data = 0;
    }
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "scoreEstimation");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("scoreEstimationCameraIp/image_raw/tf_list", 1000, estimationScore);

    //ros::Subscriber sub2 = n.subscribe("color", 1000, setSens);

    ros::Subscriber sub3 = n.subscribe("reset", 1000, reset);

    pub = n.advertise<std_msgs::Int32>("scoreCamera", 10);

    if(ros::param::has("couleur")){
    
    if(ros::param::get("couleur",sens)){
    ROS_INFO("done chooooooosing colorrrrr : %d",sens);
    
    }else{
     ROS_INFO("Canttttttt choooooooooooooose colooooooooooor");
    }
    }

    ros::Rate r(1);
    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }

    // ros::spin();

    return 0;
}
