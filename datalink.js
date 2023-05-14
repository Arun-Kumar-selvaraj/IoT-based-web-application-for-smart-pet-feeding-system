
import { initializeApp } from "https://www.gstatic.com/firebasejs/9.1.0/firebase-app.js";
import { getDatabase, ref, set, child, update, remove, onValue } from "https://www.gstatic.com/firebasejs/9.1.0/firebase-database.js";

/***** Firebase config *****/
const firebaseConfig = {
    apiKey: "AIzaSyBuAT3UJ9zZEPVsR2Oml0V1oFMHeT8CgXw",
    authDomain: "iot-feeder-1d737.firebaseapp.com",
    databaseURL: "https://iot-feeder-1d737-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "iot-feeder-1d737",
    storageBucket: "iot-feeder-1d737.appspot.com",
    messagingSenderId: "801152065411",
    appId: "1:801152065411:web:85d50665c53af8c265e0b6",
    measurementId: "G-4B4BSRMM91"
  };

/***** Initialize Firebase *****/
const app = initializeApp(firebaseConfig);

//**** write data to firebase  from the web application
function write_db() {
    console.log("DEBUG: Write function");
    var db = getDatabase();
    var create_db_table = ref(db, 'ultra value/'); //first variable name in database
    var user_name = document.getElementById("user_name").value; //user_name is ID for the status of servo motor (1 for idel state 0 for working state)
    var waterpump = document.getElementById("water").value;     // water is the ID for the status of DC pump (1 for idel state 0 for working state)
    if( user_name == '' || waterpump=='' ){
        alert("Make sure, must be non-empty data is required!!!");
        console.log("Make sure, must be non-empty data is required!!!");
        throw "Make sure, must be non-empty data is required!!!";
    }
    set(ref(db, 'ultra value/'), {
      user_name: user_name, //set the values in database
      Water_pump: waterpump
    }).then((res) => {
        console.log();
    })
    .catch((err) => {
        alert(err.message);
        console.log(err.code);
        console.log(err.message);
    })
}

/***** read data from firebase *****/
function read_db() {
    var db = getDatabase();
    var connect_db = ref(db, 'ultra value/'); //first variable name in database
    var retrieve_data='';
    console.log("DEBUG: Read function");
    onValue(connect_db, (snapshot) => {
        retrieve_data = snapshot.val();
        call_loop_print(retrieve_data);
        document.getElementById("display_read_data").innerHTML =  "<pre>" + "Distance: " + retrieve_data.us + "\n"+"level:"+retrieve_data.lvl+ "</pre>";
        }) //Between <pre> tag replace or add your sensor values
    function call_loop_print(retrieve_data){
        for (var r=0;r<Object.entries(retrieve_data).length;r++){
            var key = Object.keys(retrieve_data)[r];
            var value = retrieve_data[key];
            console.log("Key_" + r + ': ' + key + " Value_:" + r + ': ' + value );
           }
 }
}
/***** call write data function *****/
var write_data_to_firebase = document.getElementById("write_data_to_firebase");
write_data_to_firebase.addEventListener('click', write_db);


/***** call read data function *****/
var read_data_from_firebase = document.getElementById("read_data_from_firebase");
read_data_from_firebase.addEventListener('click', read_db);