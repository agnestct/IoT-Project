// src/components/StatusCard.jsx

function StatusCard({ elevator }) {

  const traj = elevator.pattern;
  const startFloor = traj & 0x0F;
  const endFloor = (traj >> 4) & 0x0F;

  return (
    <div style={styles.card}>
      <h2 style={styles.title}>Last Status</h2>
      <p>Floor: {elevator.floor}</p>
      <p>Pressure: {elevator.pressure} Pa</p>
      <p>Trajectory: Floor {startFloor} → Floor {endFloor}</p>
      <p>Timestamp: {elevator.timestamp}</p>
    </div>
  );
}

const styles = {
  card: {
    border: "1px solid #ccc",
    padding: "15px",
    marginBottom: "15px",
  },
  title: {
    marginBottom: "10px",
  },
};

export default StatusCard;
