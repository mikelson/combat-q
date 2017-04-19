function getModifier(character) {
    return character.modifier;
}

function isAware(character) {
    return character.isAware;
}
function setAware(character, value) {
    character.isAware = value;
}

function isTakingReadied(character) {
    return character.isTakingReadied;
}
function setTakingReadied(character, value) {
    return character.isTakingReadied = value;
}

function isReadied(character) {
    return character.isReadied;
}
function setReadied(character, value) {
    character.isReadied = value;
}

function isDelaying(character) {
    return character.isDelaying;
}
function setDelaying(character, value) {
    character.isDelaying = value;
}

export {
    getModifier,
    isAware,
    setAware,
    isTakingReadied,
    setTakingReadied,
    isReadied,
    setReadied,
    isDelaying,
    setDelaying,
};