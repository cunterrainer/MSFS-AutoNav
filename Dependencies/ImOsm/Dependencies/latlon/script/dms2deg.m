function deg = dms2deg(d,m,s, nsew)
    deg = d + m/60 + s/3600;
    if nargin > 3
        if  nsew == 's' || nsew == 'S' || nsew == 'w' || nsew == 'W'
            deg = -deg;
        end
    end
end