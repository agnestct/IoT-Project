import { useState, useEffect } from "react";
import { ref, onValue } from "firebase/database";
import { db } from "./firebase";
import StatusCard from "./components/StatusCard";
import AlarmPanel from "./components/AlarmPanel";
import ReportChart from "./components/ReportChart";
import FlowSpikeChart from "./components/FlowSpikeChart"; 

function App() {
  const [latestElevator, setLatestElevator] = useState(null);
  const [history, setHistory] = useState([]);

  useEffect(() => {
    const elevatorRef = ref(db, "elevator");
    const unsubscribe = onValue(elevatorRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        const arr = Object.values(data); 
        setHistory(arr);
        setLatestElevator(arr[arr.length - 1]); 
      }
    });

    return () => unsubscribe();
  }, []);

  if (!latestElevator) return <div>Loading...</div>;

  return (
  <div style={containerStyle}>
    <h1 style={titleStyle}>Elevator Monitoring Dashboard</h1>
    <div style={cardContainerStyle}>
      <div style={cardStyle}>
        <StatusCard elevator={latestElevator} />
      </div>
      <div style={cardStyle}>
        <AlarmPanel elevator={latestElevator} />
      </div>
    </div>
        <h2>Elevator Floor History & Flow Spikes</h2>
        {/* 原有图表 */}
        <ReportChart history={history} />
        {/* 新增突增检测图表 */}
        <FlowSpikeChart data={history} />
  </div>
);
}

const containerStyle = {
  padding: "20px",
  fontFamily: "sans-serif",
};
const titleStyle = {
  padding: "20px",
  fontFamily: "sans-serif",
  textAlign: "center",
  
};

const cardContainerStyle = {
  display: "flex",
  flexWrap: "wrap",
  gap: "20px",
};

const cardStyle = {
  flex: "1 1 300px",
  minWidth: "250px",
};

export default App;


