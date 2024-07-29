clear

p1.lat = dms2deg(50,03,59,'N');
p1.lon = dms2deg(5,42,53,'W');
p2.lat = dms2deg(58,38,38,'N');
p2.lon = dms2deg(3,04,12,'W');

d_h = dist_haversine(p1.lat,p1.lon,p2.lat,p2.lon);
disp(['Haversine: ' num2str(d_h*1e-3)]);

d_s = dist_spherical(p1.lat,p1.lon,p2.lat,p2.lon);
disp(['Spherical: ' num2str(d_s*1e-3)]);

d_e = dist_equirect(p1.lat,p1.lon,p2.lat,p2.lon);
disp(['Equirect: ' num2str(d_e*1e-3)]);

d_f = dist_flatearth(p1.lat,p1.lon,p2.lat,p2.lon);
disp(['Flat earth: ' num2str(d_e*1e-3)]);

b = bearing(p1.lat,p1.lon,p2.lat,p2.lon);
disp(['Bearing: ' num2str(b)]);

[p3.lat, p3.lon] = midpoint(p1.lat, p1.lon, p2.lat, p2.lon);
disp(['Midpoint: ' num2str(p3.lat) ' ' num2str(p3.lon)]);

[p4.lat, p4.lon] = destination( ...
    dms2deg(53,19,14,'N'), dms2deg(1,43,47,'W'), ...
    124.8*1e3, dms2deg(96,01,18));

[p4.lat, p4.lon] = destination(p1.lat, p1.lon, d_h, b);