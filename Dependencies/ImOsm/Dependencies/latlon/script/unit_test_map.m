clear

pos1.lat = 43.58223;
pos1.lon = 39.70199;
[pos2.lat, pos2.lon] = destination(pos1.lat, pos1.lon, 111, 222);
[pos0.lat, pos0.lon] = midpoint(pos1.lat, pos1.lon, pos2.lat, pos2.lon);

[pos1.x, pos1.y] = latlon2local(pos1.lat,pos1.lon,0,[pos0.lat pos0.lon 0]);
[pos2.x, pos2.y] = latlon2local(pos2.lat,pos2.lon,0,[pos0.lat pos0.lon 0]);

[~,pos1.xx, pos1.yy] = dist_equirect(pos0.lat, pos0.lon, pos1.lat, pos1.lon);
[~,pos2.xx, pos2.yy] = dist_equirect(pos0.lat, pos0.lon, pos2.lat, pos2.lon);

figure(1); clf;
geoscatter(pos0.lat, pos0.lon); hold on;
geoscatter(pos1.lat, pos1.lon); 
geoscatter(pos2.lat, pos2.lon);
geolimits([43.5727 43.5887], [39.6884 39.7170]);
legend({'P0' 'P1' 'P2'});
geobasemap topographic;

disp(pos1);
disp(pos2);