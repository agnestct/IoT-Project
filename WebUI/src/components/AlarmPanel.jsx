// src/components/AlarmPanel.jsx

function AlarmPanel({ elevator }) {
  console.log(elevator.alarmflag);
  const isAlarm = !!elevator.alarmflag; 
  return (
    <div
      style={{
        border: `1px solid ${isAlarm ? "red" : "green"}`,
        padding: "15px",
        marginBottom: "15px",
      }}
    >
      <h2>Alarm Panel</h2>
      {isAlarm ? (
        <>
          <p style={{ color: "red", fontWeight: "bold" }}>⚠️ Elevator Malfunction!</p>
          <p style={{ color: "red", fontWeight: "bold" }}>⚠️ {elevator.timestamp}</p>
        </>
      ) : (
        <p style={{ color: "green", fontWeight: "bold" }}>Operating Normally</p>
      )}
    </div>
  );
}

export default AlarmPanel;
