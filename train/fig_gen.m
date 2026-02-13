fid = fopen('Data.txt','r');
data = textscan(fid, ...
    'Pressure: %f Pa, temperature: %f', ...
    'Delimiter', '\n');
fclose(fid);

pressure = data{1};      
temperature = data{2};   
t = 1:length(pressure);  

figure
yyaxis left
plot(t, pressure, '-o')
ylabel('Pressure (Pa)')

yyaxis right
plot(t, temperature, '-s')
ylabel('Temperature (°C)')

xlabel('Sample Index')
title('Pressure and Temperature')
grid on
