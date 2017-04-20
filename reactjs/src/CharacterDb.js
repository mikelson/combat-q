import {get, set} from './PerformerDb.js';

function getModifier(character) {
    return get(character, "modifier");
}

function isAware(character) {
    return get(character, "isAware");
}
function setAware(character, value) {
    set(character, "isAware", value);
}

function isTakingReadied(character) {
    return get(character, "isTakingReadied");
}
function setTakingReadied(character, value) {
    return character.isTakingReadied = value;
}

function isReadied(character) {
    return get(character, "isReadied");
}
function setReadied(character, value) {
    set(character, "isReadied", value);
}

function isDelaying(character) {
    return get(character, "isDelaying");
}
function setDelaying(character, value) {
    set(character, "isDelaying", value);
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