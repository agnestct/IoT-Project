// src/firebase.js
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
      apiKey: "AIzaSyDF8QJaXt-0qQhhKSLVvI3jCTXhvWI7UfY",
      authDomain: "elevatormonitor-3e8cd.firebaseapp.com",
      databaseURL: "https://elevatormonitor-3e8cd-default-rtdb.europe-west1.firebasedatabase.app",
      projectId: "elevatormonitor-3e8cd",
      storageBucket: "elevatormonitor-3e8cd.firebasedatabase.app",
      messagingSenderId: "701335486505",
      appId: "1:701335486505:web:152a2a5fbc563483bedc38"
    };

const app = initializeApp(firebaseConfig);
export const db = getDatabase(app);
