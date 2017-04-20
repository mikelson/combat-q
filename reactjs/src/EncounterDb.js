let getState;
let setState;

const EncounterDb = {
    setGetState: function(f) {
        getState = f;
    },
    setSetState: function(f) {
        setState = f;
    }
};

function getRound(encounter) {
	return getState().round;
};
function setRound(encounter, round) {
	setState({round});
};

function getFirst(encounter) {
	const id = getState().firstId;
	return getState().performers.find(p => p.id === id);
};
function setFirst(encounter, performer) {
	setState({
		firstId: performer && performer.id
	});
};

function getCurrent(encounter) {
	const id = getState().currentId;
	return getState().performers.find(p => p.id === id);
};
function setCurrent(encounter, performer) {
	setState({
		currentId: performer && performer.id
	});
};

export {
	EncounterDb,
    getRound,
    setRound,
    getFirst,
    setFirst,
    getCurrent,
    setCurrent
};