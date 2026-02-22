%% -------- USER SETTINGS --------
port = "COM3";      % <-- change to your Arduino port (e.g., "COM5" or "/dev/ttyACM0")
baud = 115200;
recordTime = 30;    % seconds to record

%% -------- SETUP SERIAL --------
s = serialport(port, baud);
configureTerminator(s, "LF");
flush(s);

disp("Recording started...");

%% -------- DATA STORAGE --------
t = [];
acc = [];   % columns: [ax ay az]

startTime = tic;

%% -------- READ LOOP --------
while toc(startTime) < recordTime
    line = readline(s);

    % Look for lines containing acceleration data
    if contains(line, "Scaled. Acc")
        
        % Extract numbers inside brackets
        nums = regexp(line, '[-+]?\d+\.?\d*', 'match');
        
        if numel(nums) >= 3
            ax = str2double(nums{1});
            ay = str2double(nums{2});
            az = str2double(nums{3});

            t(end+1,1) = toc(startTime);
            acc(end+1,:) = [ax ay az];
        end
    end
end

disp("Recording finished.");

%% -------- CLOSE SERIAL --------
clear s

%% -------- PLOT --------
figure;
plot(t, acc(:,1), 'r', t, acc(:,2), 'g', t, acc(:,3), 'b');
xlabel("Time (s)");
ylabel("Acceleration (mg)");
legend("Ax","Ay","Az");
title("ICM-20948 Acceleration Data");
grid on;

%% -------- SAVE DATA --------
save("imu_data_2nd_to_5th_floor.mat", "t", "acc");
%writematrix([t acc], "imu_data_2nd_to_5th_floor.mat.csv");