const Performers = [];

function DmNumRecordsInCategory() {
    return Performers.length;
}

/*
 * Get pointer to the object associated with a database entry. 
 * Input argument iEncounter - index of object in encounter database. 
 * Returns - reference to object. NULL if there is no entry for that index.
 */
function queryPerformerP(iEncounter) {
    return Performers[iEncounter];
}

function getPrev(performer) {
    return performer.prev;
}
function setPrev(performer, value) {
    performer.prev = value;
}

function getNext(performer) {
    return performer.next;
}
function setNext(performer, value) {
    performer.next = value;
}

function isActive(performer) {
    return performer.isActive;
}
function setActive(performer, value) {
    performer.isActive = value;
}

function getInitiative(performer) {
    return performer.initiative;
}
function setInitiative(performer, value) {
    performer.initiative = value;
}

function getNameHandle(performer) {
    return performer.name;
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