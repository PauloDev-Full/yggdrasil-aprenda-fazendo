const API_URL = "http://localhost:8080";
const viewBox = "0 0 900 640";

async function loadAndRenderTree() {
    try {
        const response = await fetch(`${API_URL}/tree`);
        
        const rawData = await response.json();
        
        const cleanData = prepareData(rawData);
        
        renderTree(cleanData);
        
    } catch (error) {

        console.error("Erro ao conectar com o backend Yggdrasil:", error);
        document.getElementById("status").innerText = "ᚲ Conexão falhou: O servidor Yggdrasil está adormecido.";
    }
}

async function insertNode(value) {
    try {

        const response = await fetch(`${API_URL}/insert?value=${value}`, {
            method: 'POST'
        });
        
        const rawData = await response.json();
        
        const cleanData = prepareData(rawData);
        renderTree(cleanData);
        
        document.getElementById("status").innerText = "ᚱ Nó invocado com sucesso.";
    } catch (error) {
        console.error("Erro na inserção:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao invocar nó. O servidor quebrou a conexão.";
    }
}

async function deleteNode(value) {
    try {

        const response = await fetch(`${API_URL}/delete?value=${value}`, {
            method: 'POST'
        });
        
        const rawData = await response.json();
        const cleanData = prepareData(rawData);
        renderTree(cleanData);
        
        document.getElementById("status").innerText = "ᚱ Nó banido da árvore.";
    } catch (error) {
        console.error("Erro na remoção:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao deletar nó. Servidor inacessível.";
    }
}

async function rotateNode(value, type) {
    try {

        const response = await fetch(`${API_URL}/rotate?value=${value}&type=${type}`, {
            method: 'POST'
        });
        
        const rawData = await response.json();
        const cleanData = prepareData(rawData);
        renderTree(cleanData);
        
        document.getElementById("status").innerText = "ᚹ Rotação manual aplicada com sucesso.";
    } catch (error) {
        console.error("Erro na rotação:", error);
        document.getElementById("status").innerText = "ᚦ Falha na rotação. Verifique os limites da subárvore.";
    }
}

async function resetTree() {
    try {

        const response = await fetch(`${API_URL}/reset`, {
            method: 'POST'
        });
        
        const rawData = await response.json();
        
        const cleanData = prepareData(rawData);
        renderTree(cleanData);
        
        document.getElementById("status").innerText = "ᚠ A árvore do mundo foi purificada e reiniciada.";
    }
    catch (error) {
        console.error("Erro ao resetar a árvore:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao resetar a árvore. Servidor inacessível.";
    }
}

function prepareData(node) {
    if (node === null) {
        return {
            value: "+",
            isPlaceholder: true
        };
    }
    return {
        value:node.value,
        heigth: node.heigth,
        left: node.left,
        right: node.right
    };
}

document.getElementById("insertButton").addEventListener("click", async () => {
    const inputElement = document.getElementById("val");
    const value = parseInt(inputElement.value);

    if(isNaN(value)){
        document.getElementById("status").innerText = "ᚦ Valor inválido. Insira um número inteiro.";
        return;
    }

    await insertNode(value);
    inputElement.value = "";
});

document.getElementById("deleteButton").addEventListener("click", async () => {
    const inputElement = document.getElementById("val");
    const value = parseInt(inputElement.value);

    if(isNaN(value)){
        document.getElementById("status").innerText = "ᚦ Valor inválido. Insira um número inteiro.";
        return;
    }

    await deleteNode(value);
    inputElement.value = "";
});

document.getElementById("reset").addEventListener("click", async () => {

    document.getElementById("val").value = "";
    
    await resetTree();
});
