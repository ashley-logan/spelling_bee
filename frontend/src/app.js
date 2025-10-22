console.log('frontend started');

async function checkBackend() {
    try {
        const response = await fetch('http://localhost:8080/health');
        console.log('Backend is running');
        return true;
    } catch (error) {
        console.log('ERROR: Backend not started');
    }
}