%% ---------- 串口设置 ----------
port = "COM6";         % 根据实际串口修改
baudrate = 115200;     % 根据设备波特率修改
s = serialport(port, baudrate);

configureTerminator(s,"LF");  % 如果数据以换行符结束
flush(s);

%% ---------- 实时绘图准备 ----------
figure;
h = scatter3(nan, nan, nan, 36, 'b', 'filled');  % 初始空图
xlabel('MAG X'); ylabel('MAG Y'); zlabel('MAG Z');
title('Real-time 3D Magnetometer Points');
grid on; axis equal;
view(45,30);
hold on;

%% ---------- 实时读取和绘制 ----------
while true
    if s.NumBytesAvailable > 0
        line = readline(s);  % 读取一行数据
        
        % 假设数据格式和之前一样
        nums = sscanf(line, 'Pressure: %f ACC: %f MAG Norm: %f MAG X: %f MAG Y: %f MAG Z: %f');
        if numel(nums) == 6
            x = nums(4);
            y = nums(5);
            z = nums(6);
            
            % 更新 scatter3 数据
            h.XData(end+1) = x;
            h.YData(end+1) = y;
            h.ZData(end+1) = z;
            
            drawnow limitrate;  % 实时更新图形，limitrate 提高效率
        end
    end
end
