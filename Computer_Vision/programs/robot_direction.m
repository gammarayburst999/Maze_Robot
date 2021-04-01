function [dir] = robot_direction(robot_image)
%robot_image='Robot_3.png';
I2 = imread(robot_image);
dir='>';
[robot_BW,robot_RGBImage]=robot_qr_02(I2);
robot_BW_02=bwareafilt(robot_BW,3);
robot_BW_02_center=bwareafilt(robot_BW_02,1);
robot_BW_02_small=bwareafilt(robot_BW_02,1,'smallest');
qr_stats_1 = regionprops( robot_BW_02_center, 'Centroid','Area');
qr_stats_3 = regionprops( robot_BW_02_small, 'Centroid','Area');
qr_centroids_1 = cat(1,qr_stats_1.Centroid);
qr_centroids_3 = cat(1,qr_stats_3.Centroid);
% figure(1)
% imshow(I2)
% figure(2)
% imshow(robot_BW_02_center)
% hold on
% plot(qr_centroids_1(:,1),qr_centroids_1(:,2),'b*')
% plot(qr_centroids_3(:,1),qr_centroids_3(:,2),'b*')
% hold off
if(qr_centroids_1(1)-qr_centroids_3(1)<0 && qr_centroids_1(2)-qr_centroids_3(2)<0)
   dir='v'; 
end
if(qr_centroids_1(1)-qr_centroids_3(1)<0 && qr_centroids_1(2)-qr_centroids_3(2)>0)
   dir='>'; 
end
if(qr_centroids_1(1)-qr_centroids_3(1)>0 && qr_centroids_1(2)-qr_centroids_3(2)>0)
   dir='^'; 
end
if(qr_centroids_1(1)-qr_centroids_3(1)>0 && qr_centroids_1(2)-qr_centroids_3(2)<0)
   dir='<'; 
end
end