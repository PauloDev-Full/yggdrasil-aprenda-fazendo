const API_URL = "http://localhost:8080";
const width = 900;
const height = 640; 

let noSelecionadoAtual = null;

document.addEventListener("DOMContentLoaded", () => {
    loadAndRenderTree();
    configurarEventosDosBaloes();
}); 

async function loadAndRenderTree() {
    try {
        const response = await fetch(`${API_URL}/tree`);
        const rawData = await response.json();
        renderTree(rawData);
    } catch (error) {
        console.error("Erro ao conectar com o backend Yggdrasil:", error);
        document.getElementById("status").innerText = "ᚲ Conexão falhou: O servidor Yggdrasil está adormecido.";
    }
} 

async function insertNode(value) {
    try {
        const response = await fetch(`${API_URL}/insert?value=${value}`, { method: 'POST' });
        const rawData = await response.json();
        renderTree(rawData);
        document.getElementById("status").innerText = "ᚱ Nó invocado com sucesso.";
    } catch (error) {
        console.error("Erro na inserção:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao invocar nó.";
    }
} 

async function deleteNode(value) {
    try {
        const response = await fetch(`${API_URL}/delete?value=${value}`, { method: 'POST' });
        const rawData = await response.json();
        renderTree(rawData);
        document.getElementById("status").innerText = "ᚱ Nó banido da árvore.";
        fecharBaloes();
    } catch (error) {
        console.error("Erro na remoção:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao deletar nó.";
    }
} 

async function rotateNode(value, type) {
    try {
        const response = await fetch(`${API_URL}/rotate?value=${value}&type=${type}`, { method: 'POST' });
        const rawData = await response.json();
        renderTree(rawData);
        document.getElementById("status").innerText = "ᚹ Rotação manual aplicada com sucesso.";
        fecharBaloes();
    } catch (error) {
        console.error("Erro na rotação:", error);
        document.getElementById("status").innerText = "ᚦ Falha na rotação.";
    }
} 

async function resetTree() {
    try {
        const response = await fetch(`${API_URL}/reset`, { method: 'POST' });
        const rawData = await response.json();
        renderTree(rawData);
        document.getElementById("status").innerText = "ᚠ A árvore do mundo foi purificada.";
        fecharBaloes();
    } catch (error) {
        console.error("Erro ao resetar a árvore:", error);
        document.getElementById("status").innerText = "ᚦ Falha ao resetar a árvore.";
    }
} 

function fecharBaloes() {
    d3.select("#baloes-container").style("display", "none");
    noSelecionadoAtual = null;
} 

function converterParaD3(node) {
    if (!node) return null;
    let children = [];
    if (node.left) children.push(converterParaD3(node.left));
    if (node.right) children.push(converterParaD3(node.right)); 

    return {
        name: node.value,
        height: node.height,
        left: node.left,
        right: node.right,
        children: children.length > 0 ? children : null
    };
} 

function renderTree(backendData) {
    const svg = d3.select("#tree");
    svg.selectAll("*").remove(); 
    fecharBaloes(); 

    if (!backendData) return;

    const dadosConvertidos = converterParaD3(backendData);
    const root = d3.hierarchy(dadosConvertidos);

    const treeLayout = d3.tree().size([width - 100, height - 120]);
    treeLayout(root);

    const gContainer = svg.append("g").attr("transform", "translate(50, 50)");

    gContainer.selectAll(".link")
        .data(root.links())
        .enter()
        .append("line")
        .attr("class", "link")
        .attr("x1", d => d.source.x)
        .attr("y1", d => d.source.y)
        .attr("x2", d => d.target.x)
        .attr("y2", d => d.target.y)
        .attr("stroke", "var(--line)")
        .attr("stroke-width", 2);

    const nodeGroups = gContainer.selectAll(".node")
        .data(root.descendants())
        .enter()
        .append("g")
        .attr("transform", d => `translate(${d.x}, ${d.y})`)
        .on("click", function(event, d) {
            event.stopPropagation(); 
            mostrarBaloesAcao(event, d.data.name);
        });

    nodeGroups.append("circle")
        .attr("r", 22)
        .attr("fill", "var(--node)")
        .attr("stroke", "var(--gold)")
        .attr("stroke-width", 3)
        .each(function(d) {
            const hEsq = d.data.left ? d.data.left.height : -1;
            const hDir = d.data.right ? d.data.right.height : -1;
            const fator = hEsq - hDir;
            if (Math.abs(fator) > 1) {
                d3.select(this)
                    .attr("fill", "var(--bad)")
                    .attr("stroke", "var(--bad)")
                    .attr("class", "n");
                d3.select(this.parentNode).attr("class", "bad");
            }
        });

    nodeGroups.append("text")
        .attr("dy", ".35em")
        .attr("text-anchor", "middle")
        .attr("font-family", "Georgia, serif")
        .attr("font-weight", "bold")
        .attr("fill", d => {
            const hEsq = d.data.left ? d.data.left.height : -1;
            const hDir = d.data.right ? d.data.right.height : -1;
            return Math.abs(hEsq - hDir) > 1 ? "#ffffff" : "var(--ink)";
        })
        .text(d => d.data.name);
} 

function mostrarBaloesAcao(event, nodeValue) {
    noSelecionadoAtual = nodeValue;

    d3.select("#baloes-container")
        .style("left", `${event.pageX - 70}px`)
        .style("top", `${event.pageY - 55}px`)
        .style("display", "block");
} 

function configurarEventosDosBaloes() {
    document.getElementById("btn-rot-esq").addEventListener("click", (e) => {
        e.stopPropagation();
        if (noSelecionadoAtual !== null) rotateNode(noSelecionadoAtual, 2);
    });

    document.getElementById("btn-rot-dir").addEventListener("click", (e) => {
        e.stopPropagation();
        if (noSelecionadoAtual !== null) rotateNode(noSelecionadoAtual, 1);
    });

    document.getElementById("btn-del-no").addEventListener("click", (e) => {
        e.stopPropagation();
        if (noSelecionadoAtual !== null) deleteNode(noSelecionadoAtual);
    });
}

document.addEventListener("click", () => fecharBaloes()); 

document.getElementById("add").addEventListener("click", async () => {
    const inputElement = document.getElementById("val");
    const value = parseInt(inputElement.value);
    if(isNaN(value)){
        document.getElementById("status").innerText = "ᚦ Valor inválido. Insira um número inteiro.";
        return;
    }
    await insertNode(value);
    inputElement.value = "";
}); 

document.getElementById("del").addEventListener("click", async () => {
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
