filename = 'magdata.txt';
fid = fopen(filename,'r');

Pressure = [];
ACC = [];
MAG_Norm = [];
MAG_X = [];
MAG_Y = [];
MAG_Z = [];

tline = fgetl(fid);
while ischar(tline)
    nums = sscanf(tline, 'Pressure: %f ACC: %f MAG Norm: %f MAG X: %f MAG Y: %f MAG Z: %f');
    if numel(nums) == 6
        Pressure(end+1) = nums(1);
        ACC(end+1) = nums(2);
        MAG_Norm(end+1) = nums(3);
        MAG_X(end+1) = nums(4);
        MAG_Y(end+1) = nums(5);
        MAG_Z(end+1) = nums(6);
    end
    tline = fgetl(fid);
end

fclose(fid);

figure;

subplot(3,1,1);
plot(Pressure, '-o'); title('Pressure'); xlabel('Sample'); ylabel('Pressure');

subplot(3,1,2);
plot(ACC, '-o'); title('ACC'); xlabel('Sample'); ylabel('Acceleration');

subplot(3,1,3);
plot(MAG_X, '-r', 'DisplayName','MAG X'); hold on;
plot(MAG_Y, '-g', 'DisplayName','MAG Y');
plot(MAG_Z, '-b', 'DisplayName','MAG Z');
plot(MAG_Norm, '-k', 'DisplayName','MAG Norm');
title('Magnetometer'); xlabel('Sample'); ylabel('Magnetic Field'); legend;
