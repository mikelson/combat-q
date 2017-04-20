import update from 'immutability-helper';

let getState;
let setState;

const PerformerDb = {
    setGetState: function(f) {
        getState = f;
    },
    setSetState: function(f) {
        setState = f;
    }
};

function DmNumRecordsInCategory() {
    return getState().performers.length;
}

/*
 * Get pointer to the object associated with a database entry. 
 * Input argument iEncounter - index of object in encounter database. 
 * Returns - reference to object. NULL if there is no entry for that index.
 */
function queryPerformerP(iEncounter) {
    return getState().performers[iEncounter];
}

function get(performer, key) {
    performer = getState().performers.find(p => p.id === performer.id);
    return performer && performer[key];
}

function set(performer, key, value) {
    const performers = getState().performers;
    const index = performers.findIndex(p => p.id === performer.id);
    const currentPerformer = performers[index];
    const updateObj = {};
    updateObj[key] = {$set: value};
    const updatedPerformer = update(currentPerformer, updateObj);
    setState(update(getState(), {
        performers: {$splice: [[index, 1, updatedPerformer]]}
    }));
}

function getPrev(performer) {
    const id = get(performer, "prevId");
    return getState().performers.find(p => p.id === id);
}
function setPrev(performer, value) {
    set(performer, "prevId", value && value.id);
}

function getNext(performer) {
    const id = get(performer, "nextId");
    return getState().performers.find(p => p.id === id);
}
function setNext(performer, value) {
    set(performer, "nextId", value && value.id);
}

function isActive(performer) {
    return get(performer, "isActive");
}
function setActive(performer, value) {
    set(performer, "isActive", value);
}

function getInitiative(performer) {
    return get(performer, "initiative");
}
function setInitiative(performer, value) {
    set(performer, "initiative", value);
}

function getNameHandle(performer) {
    return get(performer, "name");
}

/*
 * Insert newbie Performer after performer in the initiative list.
 */
function insertAfter(performer, newbie) {
    const oldFollower = getNext(performer);

    // set newbie's previous to this
    setPrev(newbie, performer);

    // set newbie's next to old next
    setNext(newbie, oldFollower);

    // set old follower's previous to newbie
    setPrev(oldFollower, newbie);

    // set next to newbie
    setNext(performer, newbie);
}
/*
 * Insert the newbie Performer before performer in the initiative list.
 * Argument - newbie - performer that is not in the list
 */
function insertBefore(performer, newbie) {
    const oldPrevious = getPrev(performer);

    // set newbie's next to this
    setNext(newbie, performer);

    // set newbie's previous to old previous
    setPrev(newbie, oldPrevious);

    // set old previous's next to newbie
    setNext(oldPrevious, newbie);

    // set previous to newbie
    setPrev(performer, newbie);
}

export {
    PerformerDb,
    DmNumRecordsInCategory,
    queryPerformerP, 
    getPrev, 
    setPrev, 
    getNext, 
    setNext, 
    isActive,
    setActive,
    getInitiative,
    setInitiative,
    getNameHandle,
    insertBefore,
    insertAfter,
};