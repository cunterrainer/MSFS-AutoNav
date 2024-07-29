function d = dist_spherical(lat1, lon1, lat2, lon2)
    R = 6371e3;
    RAD = pi/180;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    dlam = (lon2-lon1) * RAD;

    d = acos(sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(dlam)) * R;
end

