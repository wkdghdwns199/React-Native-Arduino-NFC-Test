// import React, {useState, useEffect} from 'react';
// import {View, Text, TouchableOpacity, StyleSheet} from 'react-native';
// import NfcManager, {NfcTech} from 'react-native-nfc-manager';

// // Pre-step, call this before any NFC operations
// NfcManager.start();

// function App() {
//   const [count, setCount] = useState(1);
//   const [uid, setUid] = useState('');

//   async function readNdef() {
//     try {
//       await NfcManager.requestTechnology(NfcTech.Ndef);
//       const tag = await NfcManager.getTag();
//       setCount(count + 1);
//       console.warn('Tag found', tag);
//       setUid(tag.id);
//     } catch (ex) {
//       console.warn('Oops!', ex);
//     } finally {
//       NfcManager.cancelTechnologyRequest();
//     }
//   }


//   return (
//     <View style={styles.wrapper}>
//       <TouchableOpacity onPress={readNdef}>
//         <Text style={styles.tit}>터치 후 스캔</Text>
//       </TouchableOpacity>
//       <Text style={styles.count}>카운트 : {count}</Text>
//       <Text style={styles.count}>UID : {uid}</Text>
//     </View>
//   );
// }

// const styles = StyleSheet.create({
//   wrapper: {
//     flex: 1,
//     justifyContent: 'center',
//     alignItems: 'center',
//   },
//   tit: {
//     marginBottom: 20,
//     fontSize: 40,
//   },
//   count: {
//     fontSize: 40,
//   },
// });

// export default App;

// import React, { useState, useEffect } from 'react';
// import { View, Text, TouchableOpacity, Linking, StyleSheet } from 'react-native';
// import NfcManager, { NfcTech } from 'react-native-nfc-manager';

// NfcManager.start();

// function App() {
//   const [count, setCount] = useState(1);
//   const [uid, setUid] = useState('');

//   async function readNdef() {
//     try {
//       const isEnabled = await NfcManager.isEnabled(); // Check if NFC is enabled
//       if (!isEnabled) {
//         // If NFC is not enabled, prompt the user to enable it
//         Linking.openURL('app-settings:nfc'); // Open NFC settings on Android
//       } else {
//         await NfcManager.requestTechnology(NfcTech.Ndef);
//         const tag = await NfcManager.getTag();
//         setCount(count + 1);
//         console.warn('Tag found', tag);
//         setUid(tag.id);
//       }
//     } catch (ex) {
//       console.warn('Oops!', ex);
//     } finally {
//       NfcManager.cancelTechnologyRequest();
//     }
//   }

//   return (
//     <View style={styles.wrapper}>
//       <TouchableOpacity onPress={readNdef}>
//         <Text style={styles.tit}>터치 후 스캔</Text>
//       </TouchableOpacity>
//       <Text style={styles.count}>카운트 : {count}</Text>
//       <Text style={styles.count}>UID : {uid}</Text>
//     </View>
//   );
// }

// const styles = StyleSheet.create({
//   wrapper: {
//     flex: 1,
//     justifyContent: 'center',
//     alignItems: 'center',
//   },
//   tit: {
//     marginBottom: 20,
//     fontSize: 40,
//   },
//   count: {
//     fontSize: 40,
//   },
// });

// export default App;

import React, { useState, useEffect } from 'react';
import { View, Text, TouchableOpacity, Linking, StyleSheet } from 'react-native';
import NfcManager, { NfcTech } from 'react-native-nfc-manager';

NfcManager.start();

function App() {
  const [count, setCount] = useState(1);
  const [uid, setUid] = useState('');

  useEffect(() => {
    const readNdef = async () => {
      try {
        const isEnabled = await NfcManager.isEnabled(); // Check if NFC is enabled
        if (!isEnabled) {
          // If NFC is not enabled, prompt the user to enable it
          await NfcManager.goToNfcSetting(); // Open NFC settings on Android
        } else {
          await NfcManager.requestTechnology(NfcTech.Ndef);
          const tag = await NfcManager.getTag();
          setCount(count + 1);
          console.warn('Tag found', tag);
          setUid(tag.id);
        }
      } catch (ex) {
        console.warn('Oops!', ex);
      } finally {
        NfcManager.cancelTechnologyRequest();
      }
    };

    // 주기적으로 NFC 스캔
    const nfcScanInterval = setInterval(() => {
      readNdef();
    }, 500); // 매 10초마다 스캔 (원하는 간격으로 조절 가능)

    return () => {
      // 언마운트 시, 스캔 인터벌 제거
      clearInterval(nfcScanInterval);
    };
  }, [count]);

  return (
    <View style={styles.wrapper}>
      <Text style={styles.tit}>NFC 정보 가져오기</Text>
      <Text style={styles.count}>카운트 : {count}</Text>
      <Text style={styles.count}>UID : {uid}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  wrapper: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  tit: {
    marginBottom: 20,
    fontSize: 40,
  },
  count: {
    fontSize: 40,
  },
});

export default App;
