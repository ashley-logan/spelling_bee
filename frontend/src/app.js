console.log('frontend started');
const API_BASE = 'http://localhost:8080';
async function checkBackend() {
    try {
        const response = await fetch('http://localhost:8080/health');
        console.log('Backend is running');
        return true;
    } catch (error) {
        console.log('ERROR: Backend not started');
    }
}

async function fetchFromBackend(endpoint) {
    const response = await fetch('${API_BASE}${endpoint}');
    const data = await response.json();
    // TODO do stuff with data
}

async function fetchHive() {
    fetchFromBackend('/api/hive');
}

async function onHiveSizeEntry() {
    const hiveSizeEntry = document.getElementById('hiveSize').value;
    if (hiveSizeEntry === '') {
        alert('Please enter your desires hive size');
        return;
    }
    const hiveSize = parseInt(hiveSizeEntry);
}